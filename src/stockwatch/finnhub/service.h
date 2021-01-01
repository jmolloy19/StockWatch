#ifndef STOCKWATCH_FINNHUB_SERVICE_H_
#define STOCKWATCH_FINNHUB_SERVICE_H_

#include <chrono>
#include <condition_variable>
#include <mutex>
#include <string>

namespace stockwatch {
namespace finnhub {

class Service {
   public:
    Service() = delete;
    Service(const std::string& api_key);
    ~Service() = default;

    std::string RequestUsSecurities();
    std::string RequestCandles(const std::string& symbol, const std::chrono::system_clock::time_point& from,
                               const std::chrono::system_clock::time_point& to);

   protected:
    void CallLimitWait();

   private:
    static constexpr std::chrono::milliseconds kTimeBetweenCalls{1010};
    const std::string api_key_;

    mutable std::mutex mutex_;

    std::condition_variable condition_var_;
    std::chrono::system_clock::time_point next_call_time_;
};

}  // namespace finnhub
}  // namespace stockwatch
#endif  // STOCKWATCH_FINNHUB_SERVICE_H_
