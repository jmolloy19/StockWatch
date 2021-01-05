#ifndef STOCKWATCH_STOCKWATCH_H_
#define STOCKWATCH_STOCKWATCH_H_

#include <vector>

#include "stockwatch/config.h"
#include "stockwatch/finnhub.h"
#include "stockwatch/postgres/data_base.h"
#include "stockwatch/stock.h"

namespace stockwatch {

class StockWatch {
   public:
    StockWatch() = delete;
    StockWatch(const Config& config);
    StockWatch(const StockWatch&) = delete;
    ~StockWatch();

    void Run();

   protected:
    void Init();
    void ProcessStocks();
    std::vector<Stock>::iterator GetNextStockToProcess();
    std::string GetStockCandlesJson(const std::string& symbol);
    void LogResults() const;

    static bool ShouldProcess(const rapidjson::Value& security);
    static bool IsListedOnNasdaqOrNyse(const rapidjson::Value& security);
    static bool HasValidSymbol(const rapidjson::Value& security);

   private:
    const Config config_;

    mutable std::mutex mutex_;

    Finnhub finnhub_;
    postgres::DataBase data_base_;
    std::vector<Stock> stocks_;
    std::vector<Stock>::iterator stocks_iterator_;
};

}  // namespace stockwatch
#endif  // STOCKWATCH_STOCKWATCH_H_
