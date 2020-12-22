#include "stockwatch/stock_watch.h"

#include "curl/curl.h"
#include "glog/logging.h"

#include "stockwatch/finnhub.h"
#include "stockwatch/stock.h"

namespace stockwatch {

StockWatch::~StockWatch() { curl_global_cleanup(); }

void StockWatch::Run() {
    Init();

    uint thread_contexts = std::thread::hardware_concurrency();
    uint max_processing_threads = thread_contexts - 1;

    LOG(INFO) << "Hardware thread context count: " << thread_contexts;

    processing_threads_.reserve(max_processing_threads);

    for (uint i = 0; i < max_processing_threads; i++) {
        processing_threads_.emplace_back(&stockwatch::StockWatch::StartProcessingWorker, this);
    }

    for (auto& thread : processing_threads_) {
        thread.join();
    }

    LOG(INFO) << "StockWatch finished successfully!";
}

void StockWatch::Init() {
    std::string json = finnhub_.RequestSymbols(Finnhub::Exchange::kUs);

    rapidjson::Document symbols;
    symbols.Parse(json.c_str());

    if (symbols.HasParseError()) {
        LOG(ERROR) << "JSON parse error: " << symbols.GetParseError();
        return;
    }

    DCHECK(symbols.IsArray());

    for (const auto& symbol : symbols.GetArray()) {
        if (IsValidSymbol(symbol)) {
            stocks_.emplace_back(symbol["symbol"].GetString());
        }
    }

    stock_itr_ = stocks_.begin();

    LOG(INFO) << "Stocks to process on exchange(US): " << stocks_.size();
}

void StockWatch::StartProcessingWorker() {
    std::vector<Stock>::iterator stock = GetNextStock();

    while (stock != stocks_.cend()) {
        std::string json = RequestCandlesJson(stock);

        stock->ParseFromJson(json);

        LOG_IF(INFO, stock->ExhibitsHighTightFlag()) << "HTF: " << stock->Symbol();
        
        stock = GetNextStock();
    }
}

std::vector<Stock>::iterator StockWatch::GetNextStock() {
    std::lock_guard<std::mutex> lock(mtx_);
    if (stock_itr_ == stocks_.end()) {
        LOG_FIRST_N(INFO, 1) << stocks_.size() << "/" << stocks_.size() << " stocks processed.";
        return stocks_.end();
    }

    LOG_EVERY_N(INFO, 100) << google::COUNTER << "/" << stocks_.size() << " stocks processed.";
    return stock_itr_++;
}

std::string StockWatch::RequestSymbolsJson(Finnhub::Exchange exchange) {
    std::lock_guard<std::mutex> lock(mtx_);
    return finnhub_.RequestSymbols(exchange);
}

std::string StockWatch::RequestCandlesJson(const std::vector<Stock>::const_iterator stock) {
    std::lock_guard<std::mutex> lock(mtx_);
    return finnhub_.RequestCandles(stock->Symbol(), NumDaysAgo(90), Now());
}

bool StockWatch::IsValidSymbol(const rapidjson::Value& symbol) {
    CHECK(symbol.HasMember("symbol"));
    CHECK(symbol.HasMember("currency"));
    CHECK(symbol.HasMember("type"));

    const char* symbol_str = symbol["symbol"].GetString();
    const char* currency_str = symbol["currency"].GetString();
    const char* type_str = symbol["type"].GetString();

    if (std::strcmp(currency_str, "USD") != 0) {
        return false;
    }

    if (std::strcmp(type_str, "EQS") != 0) {
        return false;
    }

    size_t symbol_length = std::strlen(symbol_str);

    if (symbol_length < 1 or symbol_length > 5) {
        return false;
    }

    for (size_t i = 0; i < symbol_length; ++i) {
        if (symbol_str[i] < 'A' or symbol_str[i] > 'Z') {
            return false;
        }
    }

    return true;
}

std::chrono::system_clock::time_point StockWatch::Now() {
    return std::chrono::system_clock::now();
}

std::chrono::system_clock::time_point StockWatch::NumDaysAgo(int days) {
    return std::chrono::system_clock::now() - std::chrono::hours(days * 24);
}

}  // namespace stockwatch