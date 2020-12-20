#include "finnhub/finnhub.h"

#include <chrono>
#include <fstream>

#include "glog/logging.h"
#include "google/protobuf/util/json_util.h"
#include "google/protobuf/util/time_util.h"

#include "finnhub/curl.h"

namespace sw {

bool Finnhub::GetExchangeSymbols(const GetExchangeSymbolsRequest& request, GetExchangeSymbolsResponse* response) {
    std::string api_call = BuildApiCallFromRequest(request);
    std::string json = "{\"exchangeSymbols\": " + Curl::MakeApiCall(api_call) + "}";

    auto status = JsonStringToMessage(json, response);

    VLOG_IF(1, not status.ok()) << "For exchange(" << ExchangeToString(request.exchange())
                                << "): " << status.error_message();

    return status.ok();
}

bool Finnhub::GetStockCandles(const GetStockCandlesRequest& request, GetStockCandlesResponse* response) {
    std::string api_call = BuildApiCallFromRequest(request);
    // std::string json = "{\"candles\": " + Curl::MakeApiCall(api_call) + "}";

    std::string filename("/home/jmolloy/Biometrics/StockWatch/Build/jsons/" + request.stock_symbol() + ".json");
    std::ifstream in_file(filename, std::ios::ate);
    if (not in_file.is_open() or not in_file.good()) {
        LOG(ERROR) << "Failed opening file to write to: " << filename << ": good=" << in_file.good();  
    }
    size_t size = in_file.tellg();
    in_file.seekg(0);
    std::string json;
    json.resize(size);
    in_file.read(json.data(), size);
    // std::string json = "{\"candles\": " + Curl::MakeApiCall(api_call) + "}";

    // std::ofstream out_file(filename
    // if (not out_file.is_open() or not out_file.good()) {
    // LOG(ERROR) << "Failed opening file to write to: " << filename << ": good=" << out_file.good();  
    // } else {
    //     out_file.write(json.c_str(), json.size());
    // }

    auto status = JsonStringToMessage(json, response);

    VLOG_IF(1, not status.ok()) << "For symbol(" << request.stock_symbol() << "): " << status.error_message();

    return status.ok();
}

std::string Finnhub::BuildApiCallFromRequest(const GetExchangeSymbolsRequest& request) {
    std::string api_call("https://finnhub.io/api/v1/stock/symbol");

    api_call.append("?exchange=" + ExchangeToString(request.exchange()));
    api_call.append("&token=" + std::string(kApiKey));

    return api_call;
}

std::string Finnhub::BuildApiCallFromRequest(const GetStockCandlesRequest& request) {
    std::string api_call("https://finnhub.io/api/v1/stock/candle");

    api_call.append("?symbol=" + request.stock_symbol());
    api_call.append("&from=" + std::to_string(google::protobuf::util::TimeUtil::TimestampToSeconds(request.from())));
    api_call.append("&to=" + std::to_string(google::protobuf::util::TimeUtil::TimestampToSeconds(request.to())));
    api_call.append("&resolution=" + ResolutionToString(request.resolution()));
    api_call.append("&token=" + std::string(kApiKey));

    return api_call;
}

google::protobuf::util::Status Finnhub::JsonStringToMessage(const std::string& json,
                                                            google::protobuf::Message* message) {
    auto status = google::protobuf::util::JsonStringToMessage(json, message);

    if (not status.ok()) {
        return google::protobuf::util::Status(status.code(), "Failed to convert json string to protobuf message(" +
                                                                 message->GetTypeName() +
                                                                 "): " + status.error_message().ToString());
    }

    return google::protobuf::util::Status::OK;
}

std::string Finnhub::ExchangeToString(enum Exchange exchange) {
    switch (exchange) {
        case Exchange::kUs:
            return "US";
        default:
            throw std::runtime_error("Missing or invalid argument for exchange.");
    }
}

std::string Finnhub::ResolutionToString(enum Resolution resolution) {
    switch (resolution) {
        case Resolution::kOneMin:
            return "1";
        case Resolution::kFiveMin:
            return "5";
        case Resolution::kFifteenMin:
            return "15";
        case Resolution::kThirtyMin:
            return "30";
        case Resolution::kSixtyMin:
            return "60";
        case Resolution::kDay:
            return "D";
        case Resolution::kWeek:
            return "W";
        case Resolution::kMonth:
            return "M";
        default:
            throw std::runtime_error("Missing or Invalid argument for resolution.");
    }
}

}  // namespace sw
