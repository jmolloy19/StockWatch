#ifndef STOCKWATCH_STOCKWATCH_STOCKWATCH_H_
#define STOCKWATCH_STOCKWATCH_STOCKWATCH_H_

#include <mutex>
#include <thread>
#include <vector>

#include "stockwatch/stock.h"

namespace stockwatch {

class StockWatch {
   public:
    StockWatch() = delete;
    StockWatch(const std::string& api_key);
    StockWatch(const StockWatch&) = delete;
    StockWatch& operator=(const StockWatch&) = delete;
    ~StockWatch();

    void Run();

   protected:
    void InitStockList();
    void ProcessStockList();
    std::vector<Stock>::iterator GetNextStockInList();
    void AddStockToHighTighFlags(std::vector<Stock>::const_iterator stock);

    static bool ShouldProcess(const rapidjson::Value& security);
    static bool IsListedOnNasdaqOrNyse(const rapidjson::Value& security);
    static bool HasSupportedSymbol(const rapidjson::Value& security);

    static std::chrono::system_clock::time_point Now();
    static std::chrono::system_clock::time_point NumDaysAgo(int days);

   private:
    mutable std::mutex mtx_;

    std::shared_ptr<Finnhub> finnhub_;
    std::vector<Stock> stock_list_;
    std::vector<Stock>::iterator next_stock_to_process_;
    std::vector<std::vector<Stock>::const_iterator> high_tight_flags_;
    std::vector<std::thread> additional_processing_threads_;
};

}  // namespace stockwatch
#endif  // STOCKWATCH_STOCKWATCH_STOCKWATCH_H_