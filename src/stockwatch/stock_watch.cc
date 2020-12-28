#include "stockwatch/stock_watch.h"

#include <string_view>

#include "curl/curl.h"
#include "glog/logging.h"
#include "rapidjson/error/en.h"

#include "stockwatch/util/io/io.h"

namespace stockwatch {

StockWatch::StockWatch(const std::string& api_key) : finnhub_(std::make_shared<Finnhub>(api_key)) {}

StockWatch::~StockWatch() { curl_global_cleanup(); }

void StockWatch::Run() {
    InitStockList();

    for (uint i = 1; i < std::thread::hardware_concurrency(); i++) {
        additional_processing_threads_.emplace_back(&stockwatch::StockWatch::ProcessStockList, this);
    }

    ProcessStockList();

    for (auto& thread : additional_processing_threads_) {
        thread.join();
    }
}

void StockWatch::InitStockList() {
    rapidjson::Document securities = finnhub_->RequestUsSecurities();

    for (const auto& security : securities.GetArray()) {
        if (ShouldProcess(security)) {
            stock_list_.emplace_back(security, finnhub_);
        }
    }

    next_stock_to_process_ = stock_list_.begin();
}

void StockWatch::ProcessStockList() {
    auto stock = GetNextStockInList();

    while (stock != stock_list_.end()) {
        stock->InitCandles(NumDaysAgo(150), Now());

        if (stock->ExhibitsHighTightFlag()) {
            AddStockToHighTighFlags(stock);
        }

        stock = GetNextStockInList();
    }
}

std::vector<Stock>::iterator StockWatch::GetNextStockInList() {
    std::lock_guard<std::mutex> lock(mtx_);
    LOG_EVERY_N(INFO, 100) << google::COUNTER << "/" << stock_list_.size() << " stocks processed.";

    return next_stock_to_process_ == stock_list_.end() ? stock_list_.end() : next_stock_to_process_++;
}

void StockWatch::AddStockToHighTighFlags(std::vector<Stock>::const_iterator stock) {
    std::lock_guard<std::mutex> lock(mtx_);
    LOG(INFO) << "!!!!!! HTF !!!!!!: " << stock->Symbol();
    
    high_tight_flags_.push_back(stock);
}

bool StockWatch::ShouldProcess(const rapidjson::Value& security) {
    CHECK(security.HasMember("symbol"));
    CHECK(security.HasMember("description"));
    CHECK(security.HasMember("type"));
    CHECK(security.HasMember("mic"));

    return IsListedOnNasdaqOrNyse(security) and HasSupportedSymbol(security);
}

bool StockWatch::IsListedOnNasdaqOrNyse(const rapidjson::Value& security) {
    constexpr std::string_view kNasdaqGlobalMkt = "XNMS";
    constexpr std::string_view kNasdaqCapitalMkt = "XNCM";
    constexpr std::string_view kNasdaqGlobalSelectMkt = "XNGS";
    constexpr std::string_view kNyse = "XNYS";

    std::string_view mic = security["mic"].GetString();

    return mic == kNasdaqGlobalMkt or mic == kNasdaqCapitalMkt or mic == kNasdaqGlobalSelectMkt or mic == kNyse;
}

bool StockWatch::HasSupportedSymbol(const rapidjson::Value& security) {
    std::string_view symbol = security["symbol"].GetString();

    if (symbol.size() < 1 or symbol.size() > 5) {
        return false;
    }

    for (const auto& letter : symbol) {
        if (letter < 'A' or letter > 'Z') {
            return false;
        }
    }

    return true;
}

std::chrono::system_clock::time_point StockWatch::Now() { return std::chrono::system_clock::now(); }

std::chrono::system_clock::time_point StockWatch::NumDaysAgo(int days) {
    return std::chrono::system_clock::now() - std::chrono::hours(days * 24);
}

}  // namespace stockwatch
