#include "stockwatch/finnhub.h"

#include "glog/logging.h"

#include "stockwatch/util/curl/http.h"
#include "stockwatch/util/time/time.h"

namespace stockwatch {

Finnhub::Finnhub(const std::string& api_key) : api_key_(api_key) {}

std::string Finnhub::RequestUsSecurities() {
    std::string request("https://finnhub.io/api/v1/stock/symbol");
    request.append("?exchange=US");
    request.append("&currency=USD");
    request.append("&securityType=Common%20Stock");
    request.append("&token=" + api_key_);

    CallLimitWait();

    std::string response;
    util::curl::Http::Request(request, &response);

    return response;
}

std::string Finnhub::RequestCandles(const std::string& symbol, const std::chrono::system_clock::time_point& from,
                                    const std::chrono::system_clock::time_point& to) {
    std::string request("https://finnhub.io/api/v1/stock/candle");
    request.append("?symbol=" + symbol);
    request.append("&from=" + std::to_string(util::time::ToUnixTime(from)));
    request.append("&to=" + std::to_string(util::time::ToUnixTime(to)));
    request.append("&resolution=D");
    request.append("&token=" + api_key_);

    CallLimitWait();

    std::string response;
    util::curl::Http::Request(request, &response);

    return response;
}

void Finnhub::CallLimitWait() {
    std::unique_lock<std::mutex> lock(mutex_);
    condition_var_.wait_until(lock, next_call_time_,
                              [this]() { return std::chrono::system_clock::now() > next_call_time_; });

    next_call_time_ = std::chrono::system_clock::now() + kTimeBetweenCalls;
}

}  // namespace stockwatch
