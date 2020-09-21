#ifndef STOCKWATCH_STOCKWATCH_H_
#define STOCKWATCH_STOCKWATCH_H_

#include <thread>
#include <mutex>
#include <vector>
#include <chrono>
#include <queue>
#include <condition_variable>

#include "ticker_list.pb.h"

namespace sw {

class StockWatch {
    public:
        StockWatch() = default;
        ~StockWatch();
        StockWatch(const StockWatch& s) = delete;
		StockWatch& operator=(const StockWatch &) = delete;
        
        void Init();
        void Run();
        void PrintResults() const;
    protected:
        void StartQueueingWorker();
        void StartProcessingWorker();
        void QueueNextTicker();
        std::string GetNextTicker();
        bool HasMoreStocksToQueue() const;
        std::ptrdiff_t GetNumStocksProcessed() const;

    private:
        mutable std::mutex mtx_;

        TickerList ticker_list_;
        google::protobuf::RepeatedPtrField<Ticker>::const_iterator ticker_itr_;

        std::condition_variable cv_;
        std::queue<std::string> queue_;
        std::thread queueing_thread_;
        std::vector<std::thread> processing_threads_;
        std::vector<std::string> high_tight_flags_;
        bool finished_{false};

        const unsigned int kMaxThreads{std::thread::hardware_concurrency()};
        const std::string kFinishedSignal{"Finished_Signal"};
        static constexpr int kMaxApiCallsPerMin{60};
        static constexpr std::chrono::nanoseconds kMinute{60000000000};
        static constexpr std::chrono::nanoseconds kTimeBetweenCalls{kMinute / kMaxApiCallsPerMin};
};

}  // namespace sw
#endif  // STOCKWATCH_STOCKWATCH_H_