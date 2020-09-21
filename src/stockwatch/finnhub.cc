#include "stockwatch/finnhub.h"

#include <chrono>

#include "curl/curl.h"
#include "glog/logging.h"

namespace sw {

const std::string Finnhub::kApiKey{API_KEY};

void Finnhub::GetTickerListJson(const std::string& exchange, std::string* json_buffer) {
    json_buffer->reserve(100000);
    
    const auto& request("https://finnhub.io/api/v1/stock/symbol?exchange=" + exchange + 
                        "&token=" + kApiKey);

    json_buffer->append("{\"tickers\":");
    MakeApiCall(request, json_buffer);
    json_buffer->append("}");
}

void Finnhub::GetCandleDataJson(const std::string& ticker, std::string* json_buffer) {
    json_buffer->reserve(8000);

    const auto& now = std::chrono::system_clock::now();
    const auto& ninety_days_ago = std::chrono::system_clock::time_point(now - std::chrono::hours(24 * 90));

    const auto& from = std::chrono::duration_cast<std::chrono::seconds>(ninety_days_ago.time_since_epoch());
    const auto& to = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch());

    const auto& request("https://finnhub.io/api/v1/stock/candle/?resolution=D&symbol=" + ticker + 
                        "&from=" + std::to_string(from.count()) +
                        "&to=" + std::to_string(to.count()) +
                        "&token=" + kApiKey);

    MakeApiCall(request, json_buffer);
}

/**
 * This function is the callback function that gets called by libcurl as soon as there 
 * is data received that needs to be saved.
 * @param contents 	points to delivered data 
 * @param nmemb    	the size of delievered data,
 * @param size     	is always 1
 * @param userp    	points to where delivered data will be written 
 */
size_t Finnhub::Callback(void* contents, size_t size, size_t nmemb, void* userp) {	
	try {
		static_cast<std::string*>(userp)->append(static_cast<char*>(contents), size * nmemb);
	}
	catch (std::bad_alloc& err) {
		LOG(ERROR) << "Memory allocation failed. Error: " << err.what();
		return 0;
	}
	
    return size * nmemb;
}

/**
 * Makes HTTP request for the file containing historical data and writes it to a string.
 * @param url     	url to be used for HTTP request
 * @param buffer 	string that received data will be written to
 */
void Finnhub::MakeApiCall(const std::string& url, std::string* buffer) {	
	CURL *curl;
	CURLcode res;

	curl = curl_easy_init();
	if(curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, Callback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, buffer);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
		res = curl_easy_perform(curl);

		if(res != CURLE_OK) {
			LOG(ERROR) << curl_easy_strerror(res);
		} else{
			curl_easy_cleanup(curl);
		}
	}
}
}  // namespace sw

