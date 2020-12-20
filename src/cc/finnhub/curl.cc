#include "finnhub/curl.h"

#include "curl/curl.h"
#include "glog/logging.h"

namespace sw {

std::string Curl::MakeApiCall(const std::string& api_call) {
    std::string response_buffer;

    CURL* curl = curl_easy_init();

    if (curl != nullptr) {
        curl_easy_setopt(curl, CURLOPT_URL, api_call.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, Callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_buffer);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);

        CURLcode result = curl_easy_perform(curl);

        if (result != CURLE_OK) {
            LOG(ERROR) << curl_easy_strerror(result) << ": " << api_call;
        } else {
            curl_easy_cleanup(curl);
        }
    } else {
        LOG(ERROR) << "curl_easy_init() returned nullptr: " << api_call;
    }

    return response_buffer;
}

size_t Curl::Callback(void* contents, size_t size, size_t nmemb, void* userp) {
    try {
        static_cast<std::string*>(userp)->append(static_cast<char*>(contents), size * nmemb);
    } catch (std::bad_alloc& err) {
        LOG(ERROR) << "Memory allocation failed. Error: " << err.what();
        return 0;
    }

    return size * nmemb;
}

}  // namespace sw