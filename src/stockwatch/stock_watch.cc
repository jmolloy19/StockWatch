#include "stockwatch/stock_watch.h"

#include <thread>

#include "curl/curl.h"
#include "glog/logging.h"
#include "rapidjson/error/en.h"

#include "stockwatch/util/curl/email.h"
#include "stockwatch/util/io/io.h"
#include "stockwatch/util/json/json.h"
#include "stockwatch/util/time/time.h"

namespace stockwatch {

StockWatch::StockWatch(const Config& config)
    : config_(config),
      finnhub_(config_.FinnhubApiKey()),
      data_base_(config_.DbConnectionString()) {}

StockWatch::~StockWatch() { curl_global_cleanup(); }

void StockWatch::Run() {
    config_.LogConfig();
    Init();

    std::vector<std::thread> additional_processing_threads;
    for (uint i = 1; i < std::thread::hardware_concurrency(); i++) {
        additional_processing_threads.emplace_back(&stockwatch::StockWatch::ProcessStocks, this);
    }

    ProcessStocks();

    for (auto& thread : additional_processing_threads) {
        thread.join();
    }

    LogResults();
}

void StockWatch::Init() {
    std::lock_guard<std::mutex> lock(mutex_);

    rapidjson::Document securities;
    securities.Parse(finnhub_.RequestUsSecurities().c_str());

    LOG_IF(FATAL, securities.HasParseError())
        << "JSON parse error: " << rapidjson::GetParseError_En(securities.GetParseError()) << " "
        << util::json::ToString(securities);

    DCHECK(securities.IsArray()) << util::json::ToString(securities);

    for (const auto& security : securities.GetArray()) {
        if (ShouldProcess(security)) {
            stocks_.emplace_back(security);
        }
    }

    stocks_iterator_ = stocks_.begin();
    data_base_.Init();
}

void StockWatch::ProcessStocks() {
    auto stock = GetNextStockToProcess();

    while (stock != stocks_.end()) {
        std::string json = GetStockCandlesJson(stock->Symbol());

        stock->ParseCandlesFromJson(json);

        if (data_base_.IsCurrentlyWatched(stock->Symbol())) {
            std::lock_guard<std::mutex> lock(mutex_);
            data_base_.InsertNewCandles(*stock);
        }

        if (stock->ExhibitsHighTightFlag()) {
            std::lock_guard<std::mutex> lock(mutex_);
            VLOG(1) << "HTF: " << stock->Symbol();
            data_base_.WatchStock(*stock, Pattern::kHighTightFlag);
        }

        stock = GetNextStockToProcess();
    }
}

std::vector<Stock>::iterator StockWatch::GetNextStockToProcess() {
    std::lock_guard<std::mutex> lock(mutex_);
    LOG_EVERY_N(INFO, 100) << google::COUNTER << "/" << stocks_.size() << " stocks processed.";

    return stocks_iterator_ != stocks_.end() ? stocks_iterator_++ : stocks_.end();
}

std::string StockWatch::GetStockCandlesJson(const std::string& symbol) {
    std::string json;

    if (config_.IsReadFromFile()) {
        util::io::ReadFromFile(config_.JsonsDir().string() + symbol + ".json", &json);
    } else {
        std::lock_guard<std::mutex> lock(mutex_);
        json = finnhub_.RequestCandles(symbol, util::time::NumDaysAgo(150),
                                       std::chrono::system_clock::now());
    }

    if (config_.IsWriteToFile()) {
        util::io::WriteToFile(config_.JsonsDir().string() + symbol + ".json", json);
    }

    return json;
}

bool StockWatch::ShouldProcess(const rapidjson::Value& security) {
    CHECK(security.HasMember("symbol"));
    CHECK(security.HasMember("mic"));

    return IsListedOnNasdaqOrNyse(security) and HasValidSymbol(security);
}

bool StockWatch::IsListedOnNasdaqOrNyse(const rapidjson::Value& security) {
    constexpr std::string_view kNasdaqGlobalMkt = "XNMS";
    constexpr std::string_view kNasdaqCapitalMkt = "XNCM";
    constexpr std::string_view kNasdaqGlobalSelectMkt = "XNGS";
    constexpr std::string_view kNyse = "XNYS";

    std::string_view mic = security["mic"].GetString();

    return mic == kNasdaqGlobalMkt or mic == kNasdaqCapitalMkt or mic == kNasdaqGlobalSelectMkt or
           mic == kNyse;
}

bool StockWatch::HasValidSymbol(const rapidjson::Value& security) {
    std::string_view symbol = security["symbol"].GetString();

    if (symbol.empty() or symbol.size() > 5) {
        return false;
    }

    for (const auto& letter : symbol) {
        if (letter < 'A' or letter > 'Z') {
            return false;
        }
    }

    return true;
}

void StockWatch::LogResults() const {
    LOG(INFO) << "StockWatch finished successfully!";

    const auto& newly_watched_patterns = data_base_.NewlyWatchedPatterns();

    LOG(INFO) << "----- Newly Watched Patterns(" << newly_watched_patterns.size() << ") -----";
    for (const auto& [symbol, pattern] : newly_watched_patterns) {
        LOG(INFO) << symbol << ": " << ToString(pattern);
    }

    pqxx::result watched_patterns = data_base_.QueryAllWatchedPatterns();

    LOG(INFO) << "----- All Watched Patterns(" << watched_patterns.size() << ") -----";
    for (const auto& row : watched_patterns) {
        std::string symbol = row.at("symbol").as<std::string>();
        LOG(INFO) << symbol << ": (https://finance.yahoo.com/chart/" << symbol << "/#ey)";
    }

    // std::vector<std::string> cc = {"jmolloy19@gmail.com"};
    util::curl::Email email("stockwatchresults@gmail.com", "stockwatchresults@gmail.com",
                            config_.EmailPassword(), "smtps://smtp.gmail.com:465", "Daily Watch", "Test");

    email.Send();
}

}  // namespace stockwatch
