#include "stockwatch/stock_watch.h"

#include "curl/curl.h"
#include "glog/logging.h"

#include "finnhub/finnhub.h"
#include "stockwatch/stock.h"

namespace sw {

StockWatch::~StockWatch() { curl_global_cleanup(); }

void StockWatch::Run() {
    LOG(INFO) << "Hardware thread context count: " << kMaxThreads;

    queueing_thread_ = std::thread(&sw::StockWatch::StartQueueingWorker, this);

    processing_threads_.reserve(kMaxThreads);

    for (uint i = 0; i < kMaxThreads; i++) {
        processing_threads_.emplace_back(&sw::StockWatch::StartProcessingWorker, this);
    }

    queueing_thread_.join();

    for (uint i = 0; i < processing_threads_.size(); i++) {
        processing_threads_[i].join();
    }

    LOG(INFO) << "StockWatch finished successfully!";
}

void StockWatch::StartQueueingWorker() {
    GetExchangeSymbolsRequest request;
    GetExchangeSymbolsResponse response;

    request.set_exchange(Exchange::kUs);

    Finnhub::GetExchangeSymbols(request, &response);

    LOG(INFO) << "Stocks to process on exchange(" << Finnhub::ExchangeToString(Exchange::kUs)
              << "): " << response.exchange_symbols().size();

    for (const auto& symbol : response.exchange_symbols()) {
        LOG_EVERY_N(INFO, 100) << google::COUNTER << "/" << response.exchange_symbols().size() << " processed.";

        if (not IsValidSymbol(symbol)) {
            continue;
        }

        QueueNextStock(symbol);
        cv_.notify_one();
    }

    std::lock_guard<std::mutex> lock(mtx_);
    finished_ = true;
    VLOG(1) << "Finished queueing stocks.";
    cv_.notify_all();
}

void StockWatch::QueueNextStock(const Symbol& symbol) {
    std::unique_lock<std::mutex> lock(mtx_);
    // cv_.wait_for(lock, kTimeBetweenCalls);

    queue_.emplace(symbol.symbol());
}

void StockWatch::StartProcessingWorker() {
    while (true) {
        std::unique_lock<std::mutex> lock(mtx_);
        cv_.wait(lock, [this]() { return not queue_.empty() or finished_; });

        if (queue_.empty() and finished_) {
            break;
        }

        Stock stock = queue_.front();
        queue_.pop();

        lock.unlock();
        cv_.notify_one();

        stock.Init();
        LOG_IF(INFO, stock.ExhibitsHighTightFlag()) << "HTF: " << stock.Symbol();
    }

    VLOG(1) << "Processing worker finished and ready to join.";
}

bool StockWatch::IsValidSymbol(const Symbol& symbol) const {
    if (symbol.currency() != "USD") {
        return false;
    }

    if (symbol.type() != "EQS") {
        return false;
    }

    if (symbol.symbol().size() > 5) {
        return false;
    }

    for (const char character : symbol.symbol()) {
        if (character < 'A' or character > 'Z') {
            return false;
        }
    }

    return true;
}

}  // namespace sw