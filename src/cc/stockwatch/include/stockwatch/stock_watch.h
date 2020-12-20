#ifndef STOCKWATCH_STOCKWATCH_STOCKWATCH_H_
#define STOCKWATCH_STOCKWATCH_STOCKWATCH_H_

#include <algorithm>
#include <chrono>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

#include "stockwatch/stock.h"

namespace sw {

class StockWatch {
   public:
    StockWatch() = default;
    ~StockWatch();
    StockWatch(const StockWatch& s) = delete;
    StockWatch& operator=(const StockWatch&) = delete;

    void Run();

   protected:
    void StartQueueingWorker();
    void StartProcessingWorker();
    void QueueNextStock(const Symbol& symbol);
    bool IsValidSymbol(const Symbol& symbol) const;

   private:
    static constexpr std::chrono::nanoseconds kTimeBetweenCalls{std::chrono::milliseconds(1010)};
    const unsigned int kMaxThreads{std::thread::hardware_concurrency()};

    mutable std::mutex mtx_;

    std::condition_variable cv_;
    std::queue<Stock> queue_;
    std::thread queueing_thread_;
    std::vector<std::thread> processing_threads_;
    bool finished_{false};
};

}  // namespace sw
#endif  // STOCKWATCH_STOCKWATCH_STOCKWATCH_H_