#include "curl.h"

#include "curl/curl.h"
#include "glog/logging.h"

namespace stockwatch {
namespace util {
namespace curl {

void MakeRequest(const std::string& url, std::string* response) {
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

size_t Callback(void* contents, size_t size, size_t nmemb, void* userp) {
    try {
        static_cast<std::string*>(userp)->append(static_cast<char*>(contents), size * nmemb);
    } catch (std::bad_alloc& err) {
        LOG(ERROR) << "Memory allocation failed. Error: " << err.what();
        return 0;
    }

    return size * nmemb;
}

}  // namespace curl
}  // namespace util
}  // namespace stockwatch
