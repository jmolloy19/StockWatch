#ifndef STOCKWATCH_FINNHUB_FINNHUB_H_
#define STOCKWATCH_FINNHUB_FINNHUB_H_

#include "finnhub_service.pb.h"

namespace sw {

class Finnhub {
    public:
    Finnhub() = default;
    ~Finnhub() = default;

    static bool GetExchangeSymbols(const GetExchangeSymbolsRequest& request, GetExchangeSymbolsResponse* response);
    static bool GetStockCandles(const GetStockCandlesRequest& request, GetStockCandlesResponse* response);
   
    static std::string ResolutionToString(enum Resolution resolution);
    static std::string ExchangeToString(enum Exchange exchange);

   protected:
    static std::string BuildApiCallFromRequest(const GetExchangeSymbolsRequest& request);
    static std::string BuildApiCallFromRequest(const GetStockCandlesRequest& request);

    static google::protobuf::util::Status JsonStringToMessage(const std::string& json, google::protobuf::Message* message);

    private:
    static constexpr const char* kApiKey = "btefmvv48v6qag09vpag";
};

}  // namespace sw
#endif  // STOCKWATCH_FINNHUB_FINNHUB_H_