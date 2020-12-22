#ifndef STOCKWATCH_STOCK_H_
#define STOCKWATCH_STOCK_H_

#include <string>

#include "rapidjson/document.h"

namespace stockwatch {

class Stock {
   public:
    Stock() = delete;
    Stock(const std::string& symbol) : symbol_(symbol) {}
    Stock(Stock&&) = default;
    ~Stock() = default;

    void ParseFromJson(const std::string& json);
    bool ExhibitsHighTightFlag() const;
    const std::string& Symbol() const;
    std::string ToString() const;

    protected:
    static std::string JsonDocToString(const rapidjson::Document& document);

   private:
    const std::string symbol_;
    rapidjson::Document candles_;
    bool is_valid_{false};
};

}  // namespace stockwatch
#endif  // STOCKWATCH_STOCK_H_