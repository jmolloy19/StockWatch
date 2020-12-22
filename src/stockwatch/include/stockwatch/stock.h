#ifndef STOCKWATCH_STOCK_H_
#define STOCKWATCH_STOCK_H_

#include <string>

#include "stockwatch/util/rapidjson/document.h"

namespace sw {

class Stock {
   public:
    Stock() = delete;
    Stock(const std::string& symbol) : symbol_(symbol) {}
    Stock(Stock&&) = default;
    ~Stock() = default;

    bool ParseFromJson(const std::string& json);
    bool ExhibitsHighTightFlag() const;
    const std::string& Symbol() const;
    std::string ToString() const;

   private:
    const std::string symbol_;
    rapidjson::Document candles_;
};

}  // namespace sw
#endif  // STOCKWATCH_STOCK_H_