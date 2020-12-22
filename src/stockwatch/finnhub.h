#ifndef STOCKWATCH_FINNHUB_H_
#define STOCKWATCH_FINNHUB_H_

#include <chrono>
#include <string>
#include <condition_variable>
#include <mutex>

namespace stockwatch {

class Finnhub {
   public:
    Finnhub() = delete;
    Finnhub(const std::string& api_key, int calls_per_min = 60);
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

    std::string RequestSymbols(enum Exchange exchange);
    std::string RequestCandles(const std::string& symbol,
                                      const std::chrono::system_clock::time_point& from,
                                      const std::chrono::system_clock::time_point& to);

    protected:
    void MakeApiCall(const std::string& url, std::string* response);

    static std::string ToString(enum Exchange exchange);
    static std::string ToString(enum Resolution resolution);
    static std::string ToString(const std::chrono::system_clock::time_point& time_point);

   private:
    const std::string api_key_;
    const std::chrono::nanoseconds time_between_calls_;
    
    mutable std::mutex mutex_;

    std::condition_variable cv_;
    std::chrono::system_clock::time_point next_request_time_{std::chrono::system_clock::now()};
};

}  // namespace stockwatch
#endif  // STOCKWATCH_FINNHUB_H_