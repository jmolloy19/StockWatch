#ifndef STOCKWATCH_STOCKWATCH_STOCKWATCH_H_
#define STOCKWATCH_STOCKWATCH_STOCKWATCH_H_

#include <mutex>
#include <thread>
#include <vector>

#include "rapidjson/document.h"

#include "stockwatch/finnhub.h"
#include "stockwatch/stock.h"

namespace stockwatch {

class StockWatch {
   public:
    StockWatch();
    StockWatch(const StockWatch&) = delete;
    StockWatch& operator=(const StockWatch&) = delete;
    ~StockWatch();

    void Run();

   protected:
    void Init();
    void StartProcessing();
    std::vector<Stock>::iterator GetNextStock();
    static bool ShouldProcess(const rapidjson::Value& security);

    static std::chrono::system_clock::time_point Now();
    static std::chrono::system_clock::time_point NumDaysAgo(int days);

   private:
    mutable std::mutex mtx_;

    Finnhub finnhub_;
    std::vector<Stock> stocks_;
    std::vector<Stock>::iterator stock_itr_;
};

}  // namespace stockwatch
#endif  // STOCKWATCH_STOCKWATCH_STOCKWATCH_H_