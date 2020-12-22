#include "stockwatch/finnhub.h"

#include "glog/logging.h"

#include "stockwatch/util/curl/curl.h"
#include "stockwatch/util/io/io.h"

namespace stockwatch {

Finnhub::Finnhub(const std::string& api_key, int calls_per_min) : api_key_(api_key), 
    time_between_calls_(std::chrono::minutes(1) / calls_per_min) {}

std::string Finnhub::RequestSymbols(enum Exchange exchange) {
    std::string request("https://finnhub.io/api/v1/stock/symbol");

    request.append("?exchange=" + ToString(exchange));
    request.append("&token=" + api_key_);

    std::string response;
    MakeApiCall(request, &response);
    cv_.notify_one();

    return response;
}

std::string Finnhub::RequestCandles(const std::string& symbol, const std::chrono::system_clock::time_point& from,
                                    const std::chrono::system_clock::time_point& to) {
    std::string request("https://finnhub.io/api/v1/stock/candle");

    request.append("?symbol=" + symbol);
    request.append("&from=" + ToString(from));
    request.append("&to=" + ToString(to));
    request.append("&resolution=" + ToString(Resolution::kDay));
    request.append("&token=" + api_key_);

    std::string response;

    std::string filename("/home/jmolloy/Biometrics/StockWatch/Build/jsons/" + symbol + ".json");
    // util::io::ReadFromFile(filename, &response);

    MakeApiCall(request, &response);
    cv_.notify_one();

    util::io::WriteToFile(filename, response);
    return response;
}

void Finnhub::MakeApiCall(const std::string& url, std::string* response) {
    std::unique_lock<std::mutex> lock(mutex_);
    cv_.wait_until(lock, next_request_time_);

    util::curl::MakeRequest(url, response);
    next_request_time_ += time_between_calls_;
}

std::string Finnhub::ToString(enum Exchange exchange) {
    switch (exchange) {
        case Exchange::kUs:
            return "US";
        default:
            throw std::runtime_error("Missing or invalid argument for exchange.");
    }
}

std::string Finnhub::ToString(enum Resolution resolution) {
    switch (resolution) {
        case Resolution::kOneMin:
            return "1";
        case Resolution::kFiveMin:
            return "5";
        case Resolution::kFifteenMin:
            return "15";
        case Resolution::kThirtyMin:
            return "30";
        case Resolution::kSixtyMin:
            return "60";
        case Resolution::kDay:
            return "D";
        case Resolution::kWeek:
            return "W";
        case Resolution::kMonth:
            return "M";
        default:
            throw std::runtime_error("Missing or Invalid argument for resolution.");
    }
}

std::string Finnhub::ToString(const std::chrono::system_clock::time_point& time_point) {
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(time_point.time_since_epoch()).count();
    return std::to_string(seconds);
}

}  // namespace stockwatch
