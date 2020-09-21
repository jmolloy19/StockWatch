#include "stockwatch/stock.h"

#include <algorithm>

#include "google/protobuf/util/json_util.h"
#include "glog/logging.h"

#include "stockwatch/finnhub.h"

namespace sw {

void Stock::Init(const std::string& ticker) {
	candle_.Clear();
	ticker_ = ticker;

	std::string json_buffer;
	Finnhub::GetCandleDataJson(ticker_, &json_buffer);

	google::protobuf::util::JsonParseOptions options;
	options.ignore_unknown_fields = true;

	const auto& status = google::protobuf::util::JsonStringToMessage(json_buffer, &candle_, options);

	if (not status.ok()) {
        LOG(ERROR) << "Failed to parse json string(" << ticker_ << "): " << status.message();
    }
}

void Stock::Analyze() {
	if (candle_.s() != std::string("ok")) {
		LOG(WARNING) << "Request for (" + ticker_ + ") candle data returned non-ok status.";
		return;
	}

	if (candle_.c().size() < 60) {
		return;
	}													
		
	auto most_recent = candle_.c().cend() - 1;
	auto flag_pole_bottom = std::min_element(most_recent - 60, most_recent);
	auto flag_pole_top = std::max_element(flag_pole_bottom, most_recent);

	if (*flag_pole_bottom == 0.0) {
		return;
	}

	if (*flag_pole_top / *flag_pole_bottom < 1.9) {
		return;
	}

	if (std::distance(flag_pole_top, most_recent) > 15) {
		return;
	}
	
	for (auto it = most_recent; it > flag_pole_top; it--) {
		if (*it < (*flag_pole_top * .8)) {
			return;
		}
	}

	for (const auto volume : candle_.v()) {
		if (volume == 0) {
			return;
		}
	}

	double total = 0;
	for (const auto close : candle_.c()) {
		total += close;
	}

	double avg_close = total / candle_.c().size();
	if (avg_close < 1.0) {
		return;
	}
				
	exhibits_high_tight_flag_ = true;
}

bool Stock::ExhibitsHighTightFlag() const {
	return exhibits_high_tight_flag_;
}

std::string Stock::ToString() const {
	return "Symbol: " + ticker_ + "\n" + candle_.DebugString();
}

std::string Stock::Ticker() const {
	return ticker_;
}

}  // namespace sw