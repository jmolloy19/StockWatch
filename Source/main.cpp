#include <StockWatch/StockWatch.hpp>
#include <iostream>

int main(int argc, char* argv[])
{
    std::string historicalData;
    fetchHistoricalData("AAPL", &historicalData);
    std::cout << historicalData << "\n";

    // std::cout << "Stock Ticker Symbols Extracted. Total # of Stocks to Analyze: " << (end - begin) << "\n"
	// 	 << "Analyzing Stocks for Patterns..." << "\n";

    Stock stock("AAPL");
    //std::cout << "Constructor okay\n";
    std::cout << stock;
}