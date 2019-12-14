#pragma once
#include <thread>
#include <mutex>
#include <utility/progress_bar.hpp>
#include <stockwatch/options.hpp>
#include <stockwatch/stock.hpp>

#define MAX_THREADS  10

class StockWatch
{
    public:
        StockWatch() = delete;
        StockWatch(const StockWatch& s) = delete;
		StockWatch& operator=(const StockWatch &) = delete;
        StockWatch(int argc, char* argv[]);
        ~StockWatch();
        void Run();
        void PrintStockList();
    private:
        void Init();
        void CheckStock(const std::string& stock_symbol);
        void GetStockList(std::string* stocklist_buffer);
        void ParseStockList(const std::string& stocklist_buffer);
        bool ValidStockSymbol(const std::string& stock_symbol);
        void PrintReport();
        int num_stocks_ = 0;
        std::vector<std::string> stocks_;
        std::vector<std::string> high_tight_flags_;
        std::mutex mtx_;
        Options options_;
};