#include "stockwatch/stock.h"

#include "rapidjson/error/en.h"

#include "stockwatch/util/json/json.h"

namespace stockwatch {

Stock::Stock(const rapidjson::Value& security, const std::shared_ptr<Finnhub>& finnhub)
    : symbol_(security["symbol"].GetString()),
      description_(security["description"].GetString()),
      mic_code_(security["mic"].GetString()),
      finnhub_(finnhub) {}

void Stock::InitCandles(const std::chrono::system_clock::time_point& from,
                        const std::chrono::system_clock::time_point& to) {
    ClearCandles();
    candles_ = finnhub_->RequestCandles(symbol_, from, to);

    if (candles_.IsNull()) {
        LOG(WARNING) << "Null JSON doc for candles: " << symbol_;
        return;
    }

    if (not util::json::HasAllMembers(candles_, "c", "o", "h", "l", "v", "t", "s")) {
        VLOG(1) << "Invalid candles JSON doc (" << symbol_ << "): " << util::json::ToString(candles_);
        return;
    }

    if (std::strcmp(candles_["s"].GetString(), "ok") != 0) {
        VLOG(1) << "Non \"ok\" candles status (" << symbol_ << "): " << candles_["s"].GetString();
        return;
    }

    if (not util::json::AllEqualSize(candles_["c"], candles_["o"], candles_["h"], candles_["l"], candles_["v"],
                                     candles_["t"])) {
        VLOG(1) << "Invalid candles JSON doc (" << symbol_
                << "): Different size arrays: " << util::json::ToString(candles_);
        return;
    }

    has_valid_candles_ = true;
}

bool Stock::HasAverageDollarVolume(float avg_dollar_volume) const {
    DCHECK(has_valid_candles_);

    float avg_close = util::json::Average<float>(candles_["c"].Begin(), candles_["c"].End());
    int avg_volume = util::json::Average<int>(candles_["v"].Begin(), candles_["v"].End());

    return avg_close * avg_volume >= avg_dollar_volume;
}

bool Stock::HasNumTradingDays(size_t num_trading_days) const {
    DCHECK(has_valid_candles_);
    return candles_["c"].Size() >= num_trading_days;
}

bool Stock::ExhibitsHighTightFlag() const {
    if (not has_valid_candles_) {
        return false;
    }

    if (not HasNumTradingDays(90) or not HasAverageDollarVolume(5000000)) {
        return false;
    }

    const auto& closes = candles_["c"].GetArray();
    const auto& highs = candles_["h"].GetArray();
    const auto& lows = candles_["l"].GetArray();

    const auto flag_pole_bottom = util::json::MinElement(lows.End() - 60, lows.End());
    const auto flag_pole_top = util::json::MaxElement(highs.End() - 60, highs.End());

    // Price must double within 42 trading day period.
    if (flag_pole_bottom->GetFloat() == 0 or flag_pole_top->GetFloat() / flag_pole_bottom->GetFloat() < 2.0) {
        return false;
    }

    std::ptrdiff_t flag_length = std::distance(flag_pole_top, highs.End());

    // Length of flag must be atleast 5 days and no more than 15 days
    if (flag_length < 5 or flag_length > 15) {
        return false;
    }

    float flag_pole_height = flag_pole_top->GetFloat() - flag_pole_bottom->GetFloat();

    // Closes during flag period must not drop below 80% of the flag pole height.
    for (auto itr = closes.End() - flag_length; itr != closes.End(); itr++) {
        if (itr->GetFloat() - flag_pole_bottom->GetFloat() < 0.8 * flag_pole_height) {
            return false;
        }
    }

    return true;
}

void Stock::ClearCandles() {
    candles_.SetNull();
    candles_.GetAllocator().Clear();
    has_valid_candles_ = false;
}

const std::string& Stock::Symbol() const { return symbol_; }

const std::string& Stock::Description() const { return description_; }

const std::string& Stock::MicCode() const { return mic_code_; }

const rapidjson::Document& Stock::Candles() const { return candles_; }

}  // namespace stockwatch
