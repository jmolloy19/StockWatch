#ifndef STOCKWATCH_FINNHUB_H_
#define STOCKWATCH_FINNHUB_H_

#include <chrono>
#include <condition_variable>
#include <mutex>
#include <string>

#include "rapidjson/document.h"

namespace stockwatch {

class Finnhub {
   public:
    Finnhub() = delete;
    Finnhub(const std::string& api_key);
    ~Finnhub() = default;

    rapidjson::Document RequestUsSecurities();
    rapidjson::Document RequestCandles(const std::string& symbol, const std::chrono::system_clock::time_point& from,
                                       const std::chrono::system_clock::time_point& to);

    static std::string ToString(const std::chrono::system_clock::time_point& time_point);

   protected:
    void ApiCallLimitWait();
    void MakeRequest(const std::string& url, std::string* response);
    
    static size_t Callback(void* contents, size_t size, size_t nmemb, void* userp);

   private:
    /// Finnhub says the API limit is 60 calls per min, however even at this rate, it will occasionally 
    /// complain about the api limit being reached. To prevent this, we subtract one from the max calls 
    /// allowed per min to slightly increase the time between calls.
    // static constexpr int kMaxApiCallsPerMin = 60 - 1;
    static constexpr auto kTimeBetweenCalls{std::chrono::seconds(1)};
    const std::string api_key_;

    mutable std::mutex mtx_;

    std::condition_variable cv_;
    std::chrono::system_clock::time_point next_call_time_;
};

}  // namespace stockwatch
#endif  // STOCKWATCH_FINNHUB_H_