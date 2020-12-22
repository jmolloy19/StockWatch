#ifndef STOCKWATCH_UTIL_CURL_CURL_H_
#define STOCKWATCH_UTIL_CURL_CURL_H_

#include <string>

namespace stockwatch {
namespace util {
namespace curl {

void MakeRequest(const std::string& url, std::string* response);
size_t Callback(void* contents, size_t size, size_t nmemb, void* userp);

}  // namespace curl
}  // namespace util
}  // namespace stockwatch
#endif  // STOCKWATCH_UTIL_CURL_CURL_H_
