#include "stockwatch/stock_watch.h"

#include <algorithm>

#include "curl/curl.h"
#include "glog/logging.h"
#include "rapidjson/error/en.h"

#include "stockwatch/util/io/io.h"

namespace stockwatch {

StockWatch::StockWatch() : finnhub_("btefmvv48v6qag09vpag", 60) {}

StockWatch::~StockWatch() { curl_global_cleanup(); }

void StockWatch::Run() {
    Init();

    uint thread_contexts = std::min(std::thread::hardware_concurrency(), 12U);

    LOG(INFO) << "Hardware thread contexts: " << thread_contexts;

    std::vector<std::thread> additional_processing_threads;

    for (uint i = 1; i < thread_contexts; i++) {
        additional_processing_threads.emplace_back(&stockwatch::StockWatch::StartProcessing, this);
    }

    StartProcessing();

    for (auto& thread : additional_processing_threads) {
        thread.join();
    }

    LOG(INFO) << "StockWatch finished successfully!";
}

void StockWatch::Init() {
    std::string json = finnhub_.RequestSecurities(Finnhub::Exchange::kUs);

    rapidjson::Document securities;
    securities.Parse(json.c_str());

    if (securities.HasParseError()) {
        LOG(ERROR) << "JSON parse error: " << rapidjson::GetParseError_En(securities.GetParseError()) << ": "
                   << util::io::JsonValueToString(securities);
        return;
    }

    CHECK(securities.IsArray());

    for (const auto& security : securities.GetArray()) {
        if (ShouldProcess(security)) {
            stocks_.emplace_back(security);
        } else {
            VLOG(2) << "Not processing: " << util::io::JsonValueToString(security);
        }
    }

    stock_itr_ = stocks_.begin();
    LOG(INFO) << "Stocks to process on exchange(US): " << stocks_.size();
}

void StockWatch::StartProcessing() {
    auto stock = GetNextStock();

    while (stock != stocks_.end()) {
        stock->ParseFromJson(finnhub_.RequestCandles(stock->Symbol(), NumDaysAgo(90), Now()));

        LOG_IF(INFO, stock->ExhibitsHighTightFlag()) << "!!!!!! HTF !!!!!!: " << stock->Symbol();
        stock->Clear();

        stock = GetNextStock();
    }
}

std::vector<Stock>::iterator StockWatch::GetNextStock() {
    std::lock_guard<std::mutex> lock(mtx_);
    if (stock_itr_ == stocks_.end()) {
        return stocks_.end();
    }

    LOG_EVERY_N(INFO, 100) << google::COUNTER << "/" << stocks_.size() << " stocks processed.";
    return stock_itr_++;
}

bool StockWatch::ShouldProcess(const rapidjson::Value& security) {
    CHECK(security.HasMember("symbol"));
    CHECK(security.HasMember("currency"));
    CHECK(security.HasMember("description"));
    CHECK(security.HasMember("type"));
    CHECK(security.HasMember("mic"));

    const char* mic = security["mic"].GetString();

    if (std::strcmp(mic, "XNMS") != 0 and std::strcmp(mic, "XNCM") != 0 and std::strcmp(mic, "XNGS") != 0 and
        std::strcmp(mic, "XNYS") != 0) {
        return false;
    }

    const char* symbol = security["symbol"].GetString();

    size_t symbol_length = std::strlen(symbol);

    if (symbol_length < 1 or symbol_length > 5) {
        return false;
    }

    for (size_t i = 0; i < symbol_length; ++i) {
        if (symbol[i] < 'A' or symbol[i] > 'Z') {
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
