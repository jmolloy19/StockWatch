#include <StockWatch/StockWatch.hpp>
#include <iostream>
#include <thread>
#include <chrono>

int main(int argc, char* argv[])
{
    std::vector<std::string> symbols;
    createSymbolList(&symbols);

    std::cout << "Stock Ticker Symbols Extracted. Total # of Stocks to Analyze: " << symbols.size() << "\n"
		      << "Analyzing Stocks for Patterns..." << "\n\n";
    
    int numStocks = symbols.size();
    int i = 0;
    std::vector<std::thread> threads;
    std::vector<Stock> stocks;
    threads.reserve(10);
    stocks.reserve(10);
    while(i < numStocks)
    {
        for(int thr = 0; thr < 10 && i < numStocks; thr++, i++)
        {
            stocks.emplace_back(symbols[i]);
            threads.emplace_back(&Stock::analyze, std::ref(stocks[thr]));    
        }
        for(int thr = 0; thr < threads.size(); thr++)
        {
            threads[thr].join();
        }
        threads.clear();
        stocks.clear();
        if(i % 100 == 0)
            std::cout << i << " Stocks Done\n" << std::flush;
    }
    
}