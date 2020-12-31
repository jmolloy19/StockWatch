#include "stockwatch/finnhub.h"

#include "curl/curl.h"
#include "glog/logging.h"

#include "stockwatch/util/curl/curl.h"
#include "stockwatch/util/time/time.h"

namespace stockwatch {

Finnhub::Finnhub(const std::string& api_key) : api_key_(api_key) {}

std::string Finnhub::RequestUsSecurities() {
    std::string request("https://finnhub.io/api/v1/stock/symbol");

    request.append("?exchange=US");
    request.append("&currency=USD");
    request.append("&securityType=Common%20Stock");
    request.append("&token=" + api_key_);

    ApiCallLimitWait();

    std::string response;
    util::curl::MakeRequest(request, &response);

    return response;
}

std::string Finnhub::RequestCandles(const std::string& symbol, const std::chrono::system_clock::time_point& from,
                                    const std::chrono::system_clock::time_point& to) {
    std::string request("https://finnhub.io/api/v1/stock/candle");

    request.append("?symbol=" + symbol);
    request.append("&from=" + util::time::ToString(from));
    request.append("&to=" + util::time::ToString(to));
    request.append("&resolution=D");
    request.append("&token=" + api_key_);

    ApiCallLimitWait();

    std::string response;
    util::curl::MakeRequest(request, &response);

    return response;
}

void Finnhub::ApiCallLimitWait() {
    std::unique_lock<std::mutex> lock(mtx_);
    cv_.wait_until(lock, next_call_time_, [this]() { return std::chrono::system_clock::now() > next_call_time_; });

    next_call_time_ = std::chrono::system_clock::now() + kTimeBetweenCalls;
}

}  // namespace stockwatch
