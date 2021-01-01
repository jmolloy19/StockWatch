#include "stockwatch/stock_watch.h"

#include <thread>

#include "curl/curl.h"
#include "glog/logging.h"
#include "rapidjson/error/en.h"

#include "stockwatch/util/io/io.h"
#include "stockwatch/util/json/json.h"
#include "stockwatch/util/time/time.h"

namespace stockwatch {

StockWatch::StockWatch(const Config& config)
    : config_(config), finnhub_service_(config_.FinnhubApiKey()), postgres_data_base_(config_.DbConnectionString()) {}

StockWatch::~StockWatch() { curl_global_cleanup(); }

void StockWatch::Run() {
    config_.Log();
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
    securities.Parse(finnhub_service_.RequestUsSecurities().c_str());

    LOG_IF(FATAL, securities.HasParseError())
        << "JSON parse error: " << rapidjson::GetParseError_En(securities.GetParseError()) << " "
        << util::json::ToString(securities);

    for (const auto& security : securities.GetArray()) {
        if (ShouldProcess(security)) {
            stocks_.emplace_back(security);
        }
    }

    stocks_iterator_ = stocks_.begin();
    postgres_data_base_.CreateIfNotExists();
}

void StockWatch::ProcessStocks() {
    auto stock = GetNextStock();

    while (stock != stocks_.end()) {
        std::string json = GetCandlesJson(stock->Symbol());

        stock->ParseCandlesFromJson(json);

        if (stock->ExhibitsHighTightFlag()) {
            AddToHighTighFlags(std::move(*stock));
        }

        stock = GetNextStock();
    }
}

std::vector<Stock>::iterator StockWatch::GetNextStock() {
    std::lock_guard<std::mutex> lock(mutex_);
    LOG_EVERY_N(INFO, 100) << google::COUNTER << "/" << stocks_.size() << " stocks processed.";

    return stocks_iterator_ != stocks_.end() ? stocks_iterator_++ : stocks_.end();
}

std::string StockWatch::GetCandlesJson(const std::string& symbol) {
    std::string json;

    if (config_.IsReadFromFile()) {
        util::io::ReadFromFile(config_.JsonsDir().string() + symbol + ".json", &json);
    } else {
        std::lock_guard<std::mutex> lock(mutex_);
        json = finnhub_service_.RequestCandles(symbol, util::time::NumDaysAgo(150), util::time::Now());
    }

    if (config_.IsWriteToFile()) {
        util::io::WriteToFile(config_.JsonsDir().string() + symbol + ".json", json);
    }

    return json;
}

void StockWatch::AddToHighTighFlags(Stock&& stock) {
    std::lock_guard<std::mutex> lock(mutex_);
    VLOG(1) << "HTF: " << stock.Symbol();
    postgres_data_base_.InsertHighTightFlag(stock);
    high_tight_flags_.emplace_back(std::move(stock));
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

    return mic == kNasdaqGlobalMkt or mic == kNasdaqCapitalMkt or mic == kNasdaqGlobalSelectMkt or mic == kNyse;
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
    LOG(INFO) << "----- High Tight Flags(" << high_tight_flags_.size() << ") -----";

    for (const auto& stock : high_tight_flags_) {
        LOG(INFO) << stock.Symbol() << " (https://finance.yahoo.com/chart/" << stock.Symbol() << "/#ey)";
    }
}

}  // namespace stockwatch
