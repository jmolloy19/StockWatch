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

    for(int i = 0; i < symbols.size(); i++)
    {
        Stock stock(symbols[i]);

        std::thread analysis(&Stock::checkForHTF, &stock);
        std::this_thread::sleep_for(std::chrono::seconds(10));
        if (i % 100 == 0 && i != 0)
            std::cout << i << " Stocks Done\n";
        analysis.join();
    }
    
}