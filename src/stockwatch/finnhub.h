#ifndef STOCKWATCH_FINNHUB_H_
#define STOCKWATCH_FINNHUB_H_

#include <chrono>
#include <condition_variable>
#include <mutex>
#include <string>

namespace stockwatch {

class Finnhub {
   public:
    Finnhub() = delete;
    Finnhub(const std::string& api_key, int calls_per_min);
    ~Finnhub() = default;

    enum class Exchange { kUnset, kUs };

    enum Resolution {
        kUnset = 0,
        kOneMin = 1,
        kFiveMin = 2,
        kFifteenMin = 3,
        kThirtyMin = 4,
        kSixtyMin = 5,
        kDay = 6,
        kWeek = 7,
        kMonth = 8
    };

    std::string RequestSecurities(enum Exchange exchange);
    std::string RequestCandles(const std::string& symbol, const std::chrono::system_clock::time_point& from,
                               const std::chrono::system_clock::time_point& to);

    static std::string ToString(enum Exchange exchange);
    static std::string ToString(enum Resolution resolution);
    static std::string ToString(const std::chrono::system_clock::time_point& time_point);

   protected:
    void ApiCallLimitWait();

   private:
    const std::string api_key_;
    const std::chrono::nanoseconds time_between_calls_;

    mutable std::mutex mtx_;

    std::condition_variable cv_;
    std::chrono::system_clock::time_point next_call_time_;
};

}  // namespace stockwatch
#endif  // STOCKWATCH_FINNHUB_H_