#ifndef STOCKWATCH_STOCKWATCH_STOCKWATCH_H_
#define STOCKWATCH_STOCKWATCH_STOCKWATCH_H_

#include <thread>
#include <vector>

#include "rapidjson/document.h"

#include "stockwatch/stock.h"
#include "stockwatch/finnhub.h"

namespace stockwatch {

class StockWatch {
   public:
    StockWatch() : finnhub_("btefmvv48v6qag09vpag", 60) {}
    ~StockWatch();
    StockWatch(const StockWatch& s) = delete;
    StockWatch& operator=(const StockWatch&) = delete;

    void Run();

   protected:
    void Init();
    void StartProcessingWorker();
    void StartQueueingWorker();
    std::vector<Stock>::iterator GetNextStock();
    std::string RequestSymbolsJson(enum Finnhub::Exchange exchange);
    std::string RequestCandlesJson(const std::vector<Stock>::const_iterator stock);

    static bool IsValidSymbol(const rapidjson::Value& symbol);
    static std::chrono::system_clock::time_point Now();
    static std::chrono::system_clock::time_point NumDaysAgo(int days);

   private:
    mutable std::mutex mtx_;

    std::vector<Stock> stocks_;
    std::vector<Stock>::iterator stock_itr_;
    std::vector<std::thread> processing_threads_;
    Finnhub finnhub_;
};

}  // namespace stockwatch
#endif  // STOCKWATCH_STOCKWATCH_STOCKWATCH_H_