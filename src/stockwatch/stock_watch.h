#ifndef STOCKWATCH_STOCKWATCH_STOCKWATCH_H_
#define STOCKWATCH_STOCKWATCH_STOCKWATCH_H_

#include <vector>

#include "stockwatch/finnhub.h"
#include "stockwatch/stock.h"

namespace stockwatch {

struct Options {
    std::string api_key;
    bool read_from_file{false};
    bool write_to_file{false};
};

class StockWatch {
   public:
    StockWatch() = delete;
    StockWatch(const Options& options);
    StockWatch(const StockWatch&) = delete;
    StockWatch& operator=(const StockWatch&) = delete;
    ~StockWatch();

    void Run();

   protected:
    void Init();
    void ProcessStocks();
    std::vector<Stock>::iterator GetNextStock();
    std::string GetCandlesJson(const std::string& symbol);
    void AddToHighTighFlags(Stock&& stock);
    void PrintResults() const;

    static bool ShouldProcess(const rapidjson::Value& security);
    static bool IsListedOnNasdaqOrNyse(const rapidjson::Value& security);
    static bool HasValidSymbol(const rapidjson::Value& security);

   private:
    mutable std::mutex mtx_;

    Finnhub finnhub_;
    std::vector<Stock> stocks_;
    std::vector<Stock>::iterator next_stock_to_process_;
    std::vector<Stock> high_tight_flags_;
    const Options options_;
};

}  // namespace stockwatch
#endif  // STOCKWATCH_STOCKWATCH_STOCKWATCH_H_
