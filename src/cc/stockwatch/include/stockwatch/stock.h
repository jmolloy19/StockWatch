#ifndef STOCKWATCH_STOCKWATCH_STOCK_H_
#define STOCKWATCH_STOCKWATCH_STOCK_H_

#include "finnhub_resources.pb.h"
#include "google/protobuf/timestamp.pb.h"

namespace sw {

class Stock {
   public:
    Stock() = delete;
    Stock(const std::string& symbol) : symbol_(symbol) {}
    ~Stock() = default;

    void Init();
    bool ExhibitsHighTightFlag() const;
    std::string ToString() const;
    const std::string& Symbol() const;

    protected:
    static google::protobuf::Timestamp NumDaysAgo(int num_days_ago);

   private:
    const std::string symbol_;
    Candles candles_;
};

}  // namespace sw
#endif  // STOCKWATCH_STOCKWATCH_STOCK_H_