#include "stockwatch/stock.h"

#include "glog/logging.h"
#include "google/protobuf/util/time_util.h"

#include "finnhub/finnhub.h"
#include "patterns/high_tight_flag.h"

namespace sw {

void Stock::Init() {
    GetStockCandlesRequest request;
    GetStockCandlesResponse response;

    request.set_stock_symbol(symbol_);
    request.set_resolution(Resolution::kDay);
    request.mutable_from()->CopyFrom(NumDaysAgo(90));
    request.mutable_to()->CopyFrom(google::protobuf::util::TimeUtil::GetCurrentTime());

    if (not Finnhub::GetStockCandles(request, &response)) {
        return;
    }

    if (response.candles().s() != "ok") {
        VLOG(1) << "Request for (" << symbol_ << ") candles returned status: " << response.candles().s();
        return;
    }
    
    candles_.CopyFrom(response.candles());
}

bool Stock::ExhibitsHighTightFlag() const { return HighTightFlag::ExhibitsPattern(candles_); }

std::string Stock::ToString() const { return "Symbol: " + symbol_ + "\n" + candles_.DebugString(); }

const std::string& Stock::Symbol() const { return symbol_; }

google::protobuf::Timestamp Stock::NumDaysAgo(int num_days_ago) {
    auto duration = google::protobuf::util::TimeUtil::HoursToDuration(24 * num_days_ago);
    return google::protobuf::util::TimeUtil::GetCurrentTime() - duration;
}

}  // namespace sw