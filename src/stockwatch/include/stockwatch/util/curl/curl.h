#ifndef STOCKWATCH_UTIL_CURL_CURL_H_
#define STOCKWATCH_UTIL_CURL_CURL_H_

#include "curl/curl.h"
#include "glog/logging.h"

namespace sw {

class Curl {
   public:
    Curl() = default;
    ~Curl() = default;

    static void MakeRequest(const std::string& url, std::string* response) {
        CHECK_NOTNULL(response);

        CURL* curl = curl_easy_init();

        if (curl != nullptr) {
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, Callback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, response);
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);

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

   protected:
    static size_t Callback(void* contents, size_t size, size_t nmemb, void* userp) {
        try {
            static_cast<std::string*>(userp)->append(static_cast<char*>(contents), size * nmemb);
        } catch (std::bad_alloc& err) {
            LOG(ERROR) << "Memory allocation failed. Error: " << err.what();
            return 0;
        }

        return size * nmemb;
    }
};

}  // namespace sw
#endif  // STOCKWATCH_UTIL_CURL_CURL_H_