#include "stockwatch/stock_watch.h"

#include <thread>

#include "curl/curl.h"
#include "glog/logging.h"
#include "rapidjson/error/en.h"

#include "stockwatch/util/io/io.h"
#include "stockwatch/util/json/json.h"
#include "stockwatch/util/time/time.h"

namespace stockwatch {

StockWatch::StockWatch(const Options& options) : options_(options), finnhub_(options.api_key) {}

StockWatch::~StockWatch() { curl_global_cleanup(); }

void StockWatch::Run() {
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
    std::lock_guard<std::mutex> lock(mtx_);

    rapidjson::Document securities;
    securities.Parse(finnhub_.RequestUsSecurities().c_str());

    LOG_IF(FATAL, securities.HasParseError())
        << "JSON parse error: " << rapidjson::GetParseError_En(securities.GetParseError()) << " "
        << util::json::ToString(securities);

    for (const auto& security : securities.GetArray()) {
        if (ShouldProcess(security)) {
            stocks_.emplace_back(security);
        }
    }

    next_stock_to_process_ = stocks_.begin();

    LOG(INFO) << "----- StockWatch Configuration -----";
    LOG(INFO) << "Read From File:          " << (options_.read_from_file ? "true" : "false");
    LOG(INFO) << "Write To File:           " << (options_.write_to_file ? "true" : "false");
    LOG(INFO) << "Log Verbosity:           " << FLAGS_v;
    LOG(INFO) << "API limit (calls/min):   " << 60;
    LOG(INFO) << "Exchange:                " << "US";
    LOG(INFO) << "Stocks on exchange:      " << securities.Size();
    LOG(INFO) << "MIC Codes:               " << "XNMS, XNCM, XNGS, XNYS";
    LOG(INFO) << "                         ";
    LOG(INFO) << "Total Stocks to process: " << stocks_.size();
    LOG(INFO) << "------------------------------------";
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
    std::lock_guard<std::mutex> lock(mtx_);
    LOG_EVERY_N(INFO, 100) << google::COUNTER << "/" << stocks_.size() << " stocks processed.";

    return next_stock_to_process_ == stocks_.end() ? stocks_.end() : next_stock_to_process_++;
}

std::string StockWatch::GetCandlesJson(const std::string& symbol) {
    std::string json;

    if (options_.read_from_file) {
        util::io::ReadFromFile("/home/jmolloy/Biometrics/StockWatch/Build/jsons/" + symbol + ".json", &json);
    } else {
        std::lock_guard<std::mutex> lock(mtx_);
        json = finnhub_.RequestCandles(symbol, util::time::NumDaysAgo(150), util::time::Now());
    }

    if (options_.write_to_file) {
        util::io::WriteToFile("/home/jmolloy/Biometrics/StockWatch/Build/jsons/" + symbol + ".json", json);
    }

    return json;
}

void StockWatch::AddToHighTighFlags(Stock&& stock) {
    std::lock_guard<std::mutex> lock(mtx_);
    VLOG(1) << "HTF: " << stock.Symbol();
    high_tight_flags_.emplace_back(std::move(stock));
}

bool StockWatch::ShouldProcess(const rapidjson::Value& security) {
    CHECK(security.HasMember("symbol"));
    CHECK(security.HasMember("description"));
    CHECK(security.HasMember("type"));
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
