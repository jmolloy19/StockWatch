#include "stockwatch/postgres/data_base.h"

#include "glog/logging.h"

#include "stockwatch/patterns/high_tight_flag.h"
#include "stockwatch/util/io/io.h"
#include "stockwatch/util/time/time.h"

namespace stockwatch {
namespace postgres {

const std::experimental::filesystem::path DataBase::kMigrationsDir{STOCKWATCH_MIGRATIONS_DIR};

DataBase::DataBase(const std::string& connection_string) : connection_string_(connection_string) {}

void DataBase::Init() {
    CreateDbIfNotExists();
    InitCurrentlyWatchedStocks();
}

bool DataBase::IsCurrentlyWatched(const std::string& symbol) const {
    return currently_watched_stocks_.count(symbol) == 1;
}

void DataBase::WatchStock(const Stock& stock, Pattern pattern) {
    const auto& candles = stock.Candles();
    int64_t pattern_start_timestamp{-1};
    int64_t pattern_formation_timestamp{-1};
    int64_t finished_watching_timestamp{-1};
    float breakout_price{-1};

    switch (pattern) {
        case Pattern::kHighTightFlag:
            pattern_start_timestamp = HighTightFlag::FlagPoleBottom(candles)->timestamp;
            pattern_formation_timestamp = candles.back().timestamp;
            finished_watching_timestamp = util::time::ToUnixTime(util::time::DaysFromNow(40));
            breakout_price = HighTightFlag::FlagPoleTop(candles)->high;
            break;
        default:
            LOG(FATAL) << "Unknown Pattern";
    }

    pqxx::connection connection = Connect();

    try {
        std::lock_guard<std::mutex> lock(mutex_);

        pqxx::work transaction(
            connection,
            "Insert (" + stock.Symbol() + ") into watched patterns (" + ToString(pattern) + ")");

        std::stringstream query;
        query << "INSERT INTO watched_patterns "
              << "(symbol, pattern, pattern_start_timestamp, pattern_formation_timestamp, "
                 "finished_watching_timestamp, breakout_price) "
              << "VALUES (" << transaction.quote(stock.Symbol()) << ", "
              << transaction.quote(ToString(pattern)) << ", " << pattern_start_timestamp << ", "
              << pattern_formation_timestamp << ", " << finished_watching_timestamp << ", "
              << breakout_price << ");";

        transaction.exec(query.str());
        transaction.commit();

        newly_watched_patterns_.emplace_back(stock.Symbol(), pattern);
    } catch (const pqxx::unique_violation& e) {
        VLOG(2) << stock.Symbol() << " already being watched for " << ToString(pattern);
    } catch (const std::exception& e) {
        LOG(FATAL) << "Error inserting : " << e.what();
    }

    InsertNewCandles(stock);
}

void DataBase::InsertNewCandles(const Stock& stock) {
    pqxx::connection connection = Connect();

    for (const auto& candle : stock.Candles()) {
        try {
            std::lock_guard<std::mutex> lock(mutex_);

            pqxx::work transaction(connection, "Insert (" + stock.Symbol() + ") candles");

            std::stringstream query;
            query << "INSERT INTO candles "
                  << "(symbol, close, open, high, low, volume, timestamp) "
                  << "VALUES (" << transaction.quote(stock.Symbol()) << ", " << candle.close << ", "
                  << candle.open << ", " << candle.high << ", " << candle.low << ", "
                  << candle.volume << ", " << candle.timestamp << ");";

            transaction.exec(query.str());
            transaction.commit();
        } catch (const pqxx::unique_violation& e) {
            continue;
        } catch (const std::exception& e) {
            LOG(FATAL) << "Error inserting candle: " << e.what();
        }
    }
}

const std::vector<std::pair<std::string, Pattern>>& DataBase::NewlyWatchedPatterns() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return newly_watched_patterns_;
}

pqxx::result DataBase::QueryAllWatchedPatterns() const {
    pqxx::connection connection = Connect();

    try {
        std::lock_guard<std::mutex> lock(mutex_);
        pqxx::read_transaction read_transaction(connection, "Select All Watched Patterns");

        return read_transaction.exec("SELECT * FROM watched_patterns;");

    } catch (const std::exception& e) {
        LOG(FATAL) << "Error querying all watched patterns: " << e.what();
    }
}

pqxx::connection DataBase::Connect() const {
    pqxx::connection connection(connection_string_);

    LOG_IF(FATAL, not connection.is_open())
        << "Unable to establish postgres connection (" << connection_string_ << ")";

    return connection;
}

void DataBase::CreateDbIfNotExists() const {
    pqxx::connection connection = Connect();

    try {
        std::lock_guard<std::mutex> lock(mutex_);

        pqxx::work transaction(connection, "Create Database");
        std::experimental::filesystem::directory_iterator migration_files(kMigrationsDir);

        for (const auto& file : migration_files) {
            std::string statements;
            util::io::ReadFromFile(file.path().string(), &statements);
            pqxx::result result = transaction.exec(statements);
        }

        transaction.commit();
    } catch (const std::exception& e) {
        LOG(FATAL) << "Error creating data base: " << e.what();
    }
}

void DataBase::InitCurrentlyWatchedStocks() {
    pqxx::connection connection = Connect();

    try {
        std::lock_guard<std::mutex> lock(mutex_);

        pqxx::read_transaction read_transaction(connection, "Select Currently Watched Stocks");
        pqxx::result result =
            read_transaction.exec("SELECT DISTINCT symbol FROM watched_patterns;");

        for (const auto& row : result) {
            currently_watched_stocks_.emplace(row.at("symbol").as<std::string>());
        }
    } catch (const std::exception& e) {
        LOG(FATAL) << "Error selecting currently watched stocks/patterns: " << e.what();
    }
}

}  // namespace postgres
}  // namespace stockwatch
