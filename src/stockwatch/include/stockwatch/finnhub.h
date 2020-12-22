#ifndef STOCKWATCH_FINNHUB_H_
#define STOCKWATCH_FINNHUB_H_

#include <chrono>
#include <string>
#include <condition_variable>
#include <mutex>

namespace sw {

class Finnhub {
   public:
    Finnhub() = default;
    ~Finnhub() = default;

    enum class Exchange { kUnset, kUs };

    enum Resolution {
        // One minute frequency.
        kOneMin = 0,
        // Five minute frequency.
        kFiveMin = 1,
        // Fifteen minute frequency.
        kFifteenMin = 2,
        // Thirty minute frequency.
        kThirtyMin = 3,
        // Sixty minute frequency.
        kSixtyMin = 4,
        // Daily frequency.
        kDay = 5,
        // Weekly frequency.
        kWeek = 6,
        // Monthly frequency
        kMonth = 7,
    };

    std::string RequestSymbols(enum Exchange exchange);
    std::string RequestCandles(const std::string& stock_symbol,
                                      const std::chrono::system_clock::time_point& from,
                                      const std::chrono::system_clock::time_point& to);

    static std::string ToString(enum Exchange exchange);
    static std::string ToString(enum Resolution resolution);


    protected:
    static int64_t UnixTime(const std::chrono::system_clock::time_point& time_point);
    static bool ReadFromFile(const std::string& filename, std::string* contents);

    void MakeRequest(const std::string& url, std::string* response);


   private:
    static constexpr const char* kApiKey = "btefmvv48v6qag09vpag";
    static constexpr std::chrono::nanoseconds kTimeBetweenCalls{std::chrono::seconds(1)};

    mutable std::mutex mutex_;

    std::condition_variable cv_;
    std::chrono::system_clock::time_point last_request_time_;
};

}  // namespace sw
#endif  // STOCKWATCH_FINNHUB_H_