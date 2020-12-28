#include "stockwatch/finnhub.h"

#include "curl/curl.h"
#include "glog/logging.h"
#include "rapidjson/error/en.h"

#include "stockwatch/util/io/io.h"
#include "stockwatch/util/json/json.h"

namespace stockwatch {

Finnhub::Finnhub(const std::string& api_key) : api_key_(api_key) {}

rapidjson::Document Finnhub::RequestUsSecurities() {
    std::string request("https://finnhub.io/api/v1/stock/symbol");

    request.append("?exchange=US");
    request.append("&currency=USD");
    request.append("&securityType=Common%20Stock");
    request.append("&token=" + api_key_);

    std::string response;
    MakeRequest(request, &response);

    rapidjson::Document securities;
    securities.Parse(response.c_str());

    LOG_IF(FATAL, securities.HasParseError() or not securities.IsArray())
        << "JSON parse error: " << rapidjson::GetParseError_En(securities.GetParseError()) << ": "
        << util::json::ToString(securities);

    return securities;
}

rapidjson::Document Finnhub::RequestCandles(const std::string& symbol,
                                            const std::chrono::system_clock::time_point& from,
                                            const std::chrono::system_clock::time_point& to) {
    std::string request("https://finnhub.io/api/v1/stock/candle");

    request.append("?symbol=" + symbol);
    request.append("&from=" + ToString(from));
    request.append("&to=" + ToString(to));
    request.append("&resolution=D");
    request.append("&token=" + api_key_);

    std::string response;
    MakeRequest(request, &response);

    rapidjson::Document candles;
    candles.Parse(response.c_str());

    if (candles.HasParseError()) {
        LOG(WARNING) << "JSON parse error (" << symbol << "): " << rapidjson::GetParseError_En(candles.GetParseError());
        return rapidjson::Document();
    }
    
    util::io::WriteToFile("/home/jmolloy/Biometrics/StockWatch/Build/jsons/" + symbol + ".json", candles);
    return candles;
}

void Finnhub::ApiCallLimitWait() {
    std::unique_lock<std::mutex> lock(mtx_);
    while (std::chrono::system_clock::now() < next_call_time_) {
        cv_.wait_until(lock, next_call_time_);
    }
    next_call_time_ = std::chrono::system_clock::now() + kTimeBetweenCalls;
}

std::string Finnhub::ToString(const std::chrono::system_clock::time_point& time_point) {
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(time_point.time_since_epoch()).count();
    return std::to_string(seconds);
}

void Finnhub::MakeRequest(const std::string& url, std::string* response) {
    CHECK_NOTNULL(response);

    CURL* curl = curl_easy_init();

    if (curl != nullptr) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, Callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, response);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);

        ApiCallLimitWait();

        CURLcode result = curl_easy_perform(curl);

        if (result != CURLE_OK) {
            LOG(ERROR) << curl_easy_strerror(result) << ": " << url;
        } else {
            curl_easy_cleanup(curl);
        }
    } else {
        LOG(ERROR) << "curl_easy_init() returned nullptr: " << url;
    }
}

size_t Finnhub::Callback(void* contents, size_t size, size_t nmemb, void* userp) {
    try {
        static_cast<std::string*>(userp)->append(static_cast<char*>(contents), size * nmemb);
    } catch (std::bad_alloc& err) {
        LOG(ERROR) << "Memory allocation failed. Error: " << err.what();
        return 0;
    }

    return size * nmemb;
}

}  // namespace stockwatch
