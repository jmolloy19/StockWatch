#include "stockwatch/finnhub.h"

#include <fstream>

#include "glog/logging.h"

#include "stockwatch/util/curl/curl.h"

namespace sw {

std::string Finnhub::RequestSymbols(enum Exchange exchange) {
    std::string request("https://finnhub.io/api/v1/stock/symbol");

    request.append("?exchange=" + ToString(exchange));
    request.append("&token=" + std::string(kApiKey));

    std::string response;
    MakeRequest(request, &response);
    cv_.notify_one();

    return response;
}

std::string Finnhub::RequestCandles(const std::string& stock_symbol, const std::chrono::system_clock::time_point& from,
                                const std::chrono::system_clock::time_point& to) {
    std::string request("https://finnhub.io/api/v1/stock/candle");

    request.append("?symbol=" + stock_symbol);
    request.append("&from=" + std::to_string(UnixTime(from)));
    request.append("&to=" + std::to_string(UnixTime(to)));
    request.append("&resolution=" + ToString(Resolution::kDay));
    request.append("&token=" + std::string(kApiKey));


    std::string response;

    // std::string filename("/home/jmolloy/Biometrics/StockWatch/Build/jsons/" + stock_symbol + ".json");
    // ReadFromFile(filename, &response);

    MakeRequest(request, &response);
    cv_.notify_one();

    return response;
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

int64_t Finnhub::UnixTime(const std::chrono::system_clock::time_point& time_point) {
    return std::chrono::duration_cast<std::chrono::seconds>(time_point.time_since_epoch()).count();
}

bool Finnhub::ReadFromFile(const std::string& filename, std::string* contents) {
    std::ifstream in_file(filename, std::ios::ate);

    if (not in_file.is_open() or not in_file.good()) {
        LOG(ERROR) << "Failed opening file to write to: " << filename << ": good=" << in_file.good();
        return false;
    }

    size_t size = in_file.tellg();
    in_file.seekg(0);

    contents->resize(size);
    in_file.read(contents->data(), size);

    return true;;
}

void Finnhub::MakeRequest(const std::string& url, std::string* response) {
    std::unique_lock<std::mutex> lock(mutex_);
    cv_.wait_until(lock, last_request_time_ + kTimeBetweenCalls);

    Curl::MakeRequest(url, response);
    last_request_time_ = std::chrono::system_clock::now();
}

}  // namespace sw
