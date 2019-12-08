#pragma once
#include <thread>
#include <utility/file.hpp>
#include <utility/options.hpp>
#include <stockwatch/world_trading_api.hpp>
#include <stockwatch/stock.hpp>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
// --Windows--
const std::string kStockListFilePath = ".\\"; 
#else
// --Linux/MacOS--
const std::string kStockListFilePath = "./";
#endif

const int MAX_THREADS = 10;

class StockWatch
{
    public:
        StockWatch() = delete;
        StockWatch(int argc, char* argv[]);
        void Run();
        void GetStockSymbols();
    private:
        void CheckStock(const std::string& stock_symbol);
        void ParseStockSymbols(const std::string& symbols_buffer);
        bool IsValidStockSymbol(const std::string& symbol);
        void PrintReport();
        std::vector<std::string> stock_symbols_;
        std::vector<std::string> high_tight_flags_;
        Options program_options_;
        File stocklist_file_;
};