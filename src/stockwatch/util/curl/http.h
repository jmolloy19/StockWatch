#ifndef STOCKWATCH_UTIL_CURL_HTTP_H_
#define STOCKWATCH_UTIL_CURL_HTTP_H_

#include <string>

namespace stockwatch {
namespace util {
namespace curl {

class Http {
    public:
     Http() = default;
     ~Http() = default;
     
    static void Request(const std::string& url, std::string* response);
    static size_t Callback(void* ptr, size_t size, size_t nmemb, void* userp);
};

}  // namespace curl
}  // namespace util
}  // namespace stockwatch
#endif  // STOCKWATCH_UTIL_CURL_HTTP_H_
