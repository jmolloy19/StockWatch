#include "stockwatch/stockwatch.h"

#include <iostream>
#include <iomanip>

#include "curl/curl.h"
#include "google/protobuf/util/json_util.h"
#include "glog/logging.h"

#include "stockwatch/stock.h"
#include "stockwatch/finnhub.h"

namespace sw {

StockWatch::~StockWatch() {
	curl_global_cleanup();
}

void StockWatch::Init() {
    std::string json_buffer;
    Finnhub::GetTickerListJson("US", &json_buffer);

    google::protobuf::util::JsonParseOptions options;
	options.ignore_unknown_fields = true;

	const auto& status = google::protobuf::util::JsonStringToMessage(json_buffer, &ticker_list_, options);

    if (not status.ok()) {
        LOG(ERROR) << "Failed to parse json string: " << status.message();
    }

    ticker_itr_ = ticker_list_.tickers().cbegin();
    
    LOG(INFO) << "Total stocks to scan: " << ticker_list_.tickers_size();
    LOG(INFO) << "Hardware thread contexts: " << kMaxThreads;
}

void StockWatch::Run() {
    queueing_thread_ = std::thread(&sw::StockWatch::StartQueueingWorker, this);

    processing_threads_.reserve(kMaxThreads);

    for (int i = 0; i < kMaxThreads; i++) {
        processing_threads_.emplace_back(&sw::StockWatch::StartProcessingWorker, this);
    }

    for (int i = 0; i < processing_threads_.size(); i++) {
        processing_threads_[i].join();
    }

    queueing_thread_.join();
}

void StockWatch::StartQueueingWorker() {
    while (HasMoreStocksToQueue()) {
        QueueNextTicker();
    }

    std::lock_guard<std::mutex> lock(mtx_);
    finished_ = true;
    cv_.notify_all();
}

void StockWatch::QueueNextTicker() {
    std::unique_lock<std::mutex> lock(mtx_);
    cv_.wait_for(lock, kTimeBetweenCalls);

    auto ticker = (ticker_itr_++)->symbol();

    while (ticker.find("-") != std::string::npos) {
        ticker = (ticker_itr_++)->symbol();
    }

    queue_.push(ticker);

    cv_.notify_one();
}

void StockWatch::StartProcessingWorker() {
    Stock stock;

    while (true) {
        const auto& ticker = GetNextTicker();

        if (ticker == kFinishedSignal) {
            break;
        }

        stock.Init(ticker);
        stock.Analyze();

        if (stock.ExhibitsHighTightFlag()) {
            std::lock_guard<std::mutex> lock(mtx_);
            high_tight_flags_.push_back(stock.Ticker());
        }
    }
}

bool StockWatch::HasMoreStocksToQueue() const {
    std::lock_guard<std::mutex> lock(mtx_);
    return ticker_itr_ != ticker_list_.tickers().cend();
}

std::string StockWatch::GetNextTicker() {
    std::unique_lock<std::mutex> lock(mtx_);
    cv_.wait(lock, [this]() { return not queue_.empty() or finished_; });

    if (finished_ and queue_.empty()) {
        return kFinishedSignal;
    }

    auto ticker = queue_.front();
    queue_.pop();

    LOG_EVERY_N(INFO, 50) << google::COUNTER << "/" << ticker_list_.tickers_size() << " stocks processed.";

    return ticker;
}

std::ptrdiff_t StockWatch::GetNumStocksProcessed() const {
    std::lock_guard<std::mutex> lock(mtx_);
    return std::distance(ticker_list_.tickers().cbegin(), ticker_itr_);
}

void StockWatch::PrintResults() const {
    std::lock_guard<std::mutex> lock(mtx_);
    for (const auto& ticker : high_tight_flags_) {
        LOG(INFO) << "HTF: " << ticker;
    }
}

}  // namespace sw