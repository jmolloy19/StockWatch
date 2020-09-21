#ifndef STOCKWATCH_FINNHUB_H_
#define STOCKWATCH_FINNHUB_H_

#include <string>

namespace sw {

#define API_KEY "btefmvv48v6qag09vpag"

class Finnhub {
    public:
    static void GetTickerListJson(const std::string& exchange, std::string* json_buffer);
    static void GetCandleDataJson(const std::string& ticker, std::string* json_buffer);

    protected:
    static size_t Callback(void* contents, size_t size, size_t nmemb, void* userp);
    static void MakeApiCall(const std::string& url, std::string* buffer);


    /// API Key (See instructions/examples in comments above)
    static const std::string kApiKey;
};

}  // namespace sw
#endif // STOCKWATCH_FINNHUB_H_