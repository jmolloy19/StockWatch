#ifndef STOCKWATCH_FINNHUB_CURL_H_
#define STOCKWATCH_FINNHUB_CURL_H_

#include <string>

namespace sw {

class Curl {
   public:
    Curl() = default;
    ~Curl() = default;

    static std::string MakeApiCall(const std::string& api_call);

   protected:
    static size_t Callback(void* contents, size_t size, size_t nmemb, void* userp);
};

}  // namespace sw
#endif  // STOCKWATCH_FINNHUB_CURL_H_