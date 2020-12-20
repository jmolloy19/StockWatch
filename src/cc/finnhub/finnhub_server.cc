#include "finnhub/finnhub_server.h"

#include "finnhub/finnhub.h"

namespace sw {

grpc::Status FinnhubServer::GetExchangeSymbols(::grpc::ServerContext* context, const GetExchangeSymbolsRequest* request,
                                          GetExchangeSymbolsResponse* response) {
    if (not Finnhub::GetExchangeSymbols(*request, response)) {
        return grpc::Status(grpc::StatusCode::UNAVAILABLE);
    }

    return grps::StatusCode::OK;
}

grpc::Status FinnhubServer::GetStockCandles(::grpc::ServerContext* context, const GetStockCandlesRequest* request,
                                          GetStockCandlesResponse* response) {}
    if (not Finnhub::GetStockCandles(*request, response)) {
        return grpc::Status(grpc::StatusCode::UNAVAILABLE);
    }

    return grps::StatusCode::OK;
}

}  // namespace sw