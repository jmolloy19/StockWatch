#ifndef STOCKWATCH_STOCK_H_
#define STOCKWATCH_STOCK_H_

#include <string>

#include "rapidjson/document.h"

namespace stockwatch {

class Stock {
   public:
    Stock() = delete;
    Stock(const rapidjson::Value& security);
    Stock(Stock&&) = default;
    ~Stock() = default;

    void ParseFromJson(const std::string& json);
    bool ExhibitsHighTightFlag() const;
    void Clear();

    const std::string& Symbol() const;
    const std::string& Description() const;
    const std::string& MicCode() const;
    const std::string& SecurityType() const;

   private:
    const std::string symbol_;
    const std::string description_;
    const std::string mic_code_;
    const std::string security_type_;

    rapidjson::Document candles_;
    bool has_valid_candles_{false};
};

}  // namespace stockwatch
#endif  // STOCKWATCH_STOCK_H_