#include "stockwatch/stock.h"

#include "rapidjson/error/en.h"

#include "stockwatch/patterns/high_tight_flag.h"
#include "stockwatch/util/json/json.h"

namespace stockwatch {

Stock::Stock(const rapidjson::Value& security)
    : symbol_(security["symbol"].GetString()),
      mic_code_(security["mic"].GetString()) {}

void Stock::ParseCandlesFromJson(const std::string& json) {
    rapidjson::Document document;
    document.Parse(json.c_str());

    if (document.HasParseError()) {
        VLOG(1) << "JSON parse error (" << symbol_ << "): " << rapidjson::GetParseError_En(document.GetParseError());
        return;
    }

    if (document.HasMember("s") and std::strcmp(document["s"].GetString(), "ok") != 0) {
        VLOG(1) << "Non \"ok\" candles status (" << symbol_ << "): " << document["s"].GetString();
        return;
    }

    if (not util::json::HasAllMembers(document, "c", "o", "h", "l", "v", "t", "s")) {
        VLOG(1) << "Invalid JSON doc for candles (" << symbol_ << "): " << util::json::ToString(document);
        return;
    }

    if (not util::json::AllEqualSize(document["c"], document["o"], document["h"], document["l"], document["v"],
                                     document["t"])) {
        VLOG(1) << "Invalid JSON doc for candles (" << symbol_
                << "): Different size arrays: " << util::json::ToString(document);
        return;
    }

    JsonDocToCandles(document);
}

bool Stock::ExhibitsHighTightFlag() const {
    if (not HasNumTradingDays(90) or not HasAverageDollarVolume(5000000)) {
        return false;
    }

    return patterns::HighTightFlag::ExhibitsPattern(candles_);
}

const std::string& Stock::Symbol() const { return symbol_; }

const std::string& Stock::MicCode() const { return mic_code_; }

const std::vector<Candle>& Stock::Candles() const { return candles_; }

bool Stock::HasAverageDollarVolume(float avg_dollar_volume) const {
    float avg_close = AverageClose(candles_.cbegin(), candles_.cend());
    int avg_volume = AverageVolume(candles_.cbegin(), candles_.cend());

    return avg_close * avg_volume >= avg_dollar_volume;
}

bool Stock::HasNumTradingDays(size_t num_trading_days) const { return candles_.size() >= num_trading_days; }

void Stock::JsonDocToCandles(const rapidjson::Document& document) {
    const auto& closes = document["c"].GetArray();
    const auto& opens = document["o"].GetArray();
    const auto& highs = document["h"].GetArray();
    const auto& lows = document["l"].GetArray();
    const auto& volumes = document["v"].GetArray();
    const auto& timestamps = document["t"].GetArray();

    candles_.clear();

    size_t size = closes.Size();
    for (size_t i = 0; i < size; i++) {
        candles_.emplace_back(closes[i].GetFloat(), opens[i].GetFloat(), highs[i].GetFloat(), lows[i].GetFloat(),
                              volumes[i].GetInt(), timestamps[i].GetInt64());
    }
}

}  // namespace stockwatch
