#include "stockwatch/postgres/data_base.h"

#include "glog/logging.h"

#include "stockwatch/patterns/high_tight_flag.h"
#include "stockwatch/util/io/io.h"

namespace stockwatch {
namespace postgres {

const std::experimental::filesystem::path DataBase::kMigrationsDir{STOCKWATCH_MIGRATIONS_DIR};

DataBase::DataBase(const std::string& connection_string) : connection_(connection_string) {
    VLOG_IF(1, not connection_.is_open()) << "Unable to establish postgres connection (" << connection_string << ")";
}

void DataBase::CreateIfNotExists() {
    std::lock_guard<std::mutex> lock(mutex_);

    try {
        pqxx::work transaction(connection_, "Create Database");

        std::string commands;
        for (const auto& up_migration : std::experimental::filesystem::directory_iterator(kMigrationsDir)) {
            util::io::ReadFromFile(up_migration.path().string(), &commands);
            pqxx::result result = transaction.exec(commands);
        }

        transaction.commit();
    } catch (const std::exception& e) {
        LOG(FATAL) << "Error creating data base: " << e.what();
    }
}

void DataBase::InsertHighTightFlag(const Stock& stock) {
    std::lock_guard<std::mutex> lock(mutex_);

    auto flag_pole_bottom = patterns::HighTightFlag::FlagPoleBottom(stock.Candles());
    auto flag_pole_top = patterns::HighTightFlag::FlagPoleTop(stock.Candles());

    std::stringstream insert_command;

    try {
        pqxx::work transaction(connection_, "Insert (" + stock.Symbol() + ") into High Tight Flags");

        insert_command << "INSERT INTO high_tight_flags "
                       << "(symbol, flag_pole_bottom_timestamp, flag_pole_top_timestamp, breakout_price) "
                       << "VALUES (" << transaction.quote(stock.Symbol()) << ", " << flag_pole_bottom->timestamp << ", "
                       << flag_pole_top->timestamp << ", " << flag_pole_top->high << ");";

        transaction.exec(insert_command.str());
        transaction.commit();
    } catch (const pqxx::unique_violation& e) {
        VLOG(2) << "Stock (" << stock.Symbol() << ") already in high tight flag table.";
    } catch (const std::exception& e) {
        LOG(FATAL) << "Error inserting high tight flag: " << e.what() << "QUERY: " << insert_command.str();
    }
}

}  // namespace postgres
}  // namespace stockwatch
