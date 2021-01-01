#ifndef STOCKWATCH_STOCK_H_
#define STOCKWATCH_STOCK_H_

#include <chrono>
#include <memory>

#include "rapidjson/document.h"

#include "stockwatch/candle.h"

namespace stockwatch {

class Stock {
   public:
    Stock() = delete;
    Stock(const rapidjson::Value& security);
    Stock(Stock&&) = default;
    ~Stock() = default;

    void ParseCandlesFromJson(const std::string& json);
    bool ExhibitsHighTightFlag() const;

    const std::string& Symbol() const;
    const std::string& MicCode() const;
    const std::vector<Candle>& Candles() const;

   protected:
    bool HasAverageDollarVolume(float avg_dollar_volume) const;
    bool HasNumTradingDays(size_t num_trading_days) const;
    void JsonDocToCandles(const rapidjson::Document& document);

   private:
    const std::string symbol_;
    const std::string mic_code_;

    std::vector<Candle> candles_;
};

}  // namespace stockwatch
#endif  // STOCKWATCH_STOCK_H_
