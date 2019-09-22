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
    
    int count = symbols.size();
    int i = 0;
    std::vector<std::thread> threads;
    threads.reserve(count);
    while(i < count)
    {
        for(int j = 0; j < 10; j++, i++)
        {
            threads.emplace_back(analyze, std::ref(symbols[i]));    
        }
        for(int j = 10; j > 0; j--)
        {
            threads[i - j].join();
        }
        if(i % 100 == 0)
            std::cout << i << "Stocks Done";
    }
    
}