#ifndef STOCKWATCH_STOCK_H_
#define STOCKWATCH_STOCK_H_

#include "rapidjson/document.h"

#include "stockwatch/finnhub.h"

namespace stockwatch {

class Stock {
   public:
    Stock() = delete;
    Stock(const rapidjson::Value& security, const std::shared_ptr<Finnhub>& finnhub);
    Stock(Stock&&) = default;
    ~Stock() = default;

    void InitCandles(const std::chrono::system_clock::time_point& from,
                     const std::chrono::system_clock::time_point& to);

    bool ExhibitsHighTightFlag() const;

    const std::string& Symbol() const;
    const std::string& Description() const;
    const std::string& MicCode() const;
    const rapidjson::Document& Candles() const;

   protected:
    void ClearCandles();

    bool HasAverageDollarVolume(float avg_dollar_volume) const;
    bool HasNumTradingDays(size_t num_trading_days) const;


   private:
    const std::string symbol_;
    const std::string description_;
    const std::string mic_code_;

    rapidjson::Document candles_;
    std::shared_ptr<Finnhub> finnhub_;
    bool has_valid_candles_{false};
};

}  // namespace stockwatch
#endif  // STOCKWATCH_STOCK_H_
