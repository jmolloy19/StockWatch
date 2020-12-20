#ifndef STOCKWATCH_FINNHUB_FINNHUB_SERVER_H_
#define STOCKWATCH_FINNHUB_FINNHUB_SERVER_H_

#include "grpcpp/grpcpp.h"

#include "finnhub_service.grpc.pb.h"

namespace sw {

class FinnhubServer : public FinnhubService::Service {
   public:
    grpc::Status GetExchangeSymbols(::grpc::ServerContext* context, const GetExchangeSymbolsRequest* request,
                                     GetExchangeSymbolsResponse* response) override;

    grpc::Status GetStockCandles(::grpc::ServerContext* context, const GetStockCandlesRequest* request,
                                  GetStockCandlesResponse* response) override;

    void StartServer() {}
    void StopServer() {}
};

}  // namespace sw
#endif  // STOCKWATCH_FINNHUB_FINNHUB_SERVER_H_