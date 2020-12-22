#ifndef STOCKWATCH_STOCKWATCH_STOCKWATCH_H_
#define STOCKWATCH_STOCKWATCH_STOCKWATCH_H_

#include <thread>
#include <vector>

#include "stockwatch/stock.h"
#include "stockwatch/finnhub.h"
#include "stockwatch/util/rapidjson/document.h"

namespace sw {

class StockWatch {
   public:
    StockWatch() = default;
    ~StockWatch();
    StockWatch(const StockWatch& s) = delete;
    StockWatch& operator=(const StockWatch&) = delete;

    void Run();

   protected:
    void Init();
    void StartProcessingWorker();
    std::vector<Stock>::iterator GetNextStock();

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

}  // namespace sw
#endif  // STOCKWATCH_STOCKWATCH_STOCKWATCH_H_