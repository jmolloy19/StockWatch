#include <StockWatch/StockWatch.hpp>
#include <iostream>

void analyze(const std::string& symbol)
{
    Stock stock(symbol);
    if(stock.checkForHTF())
        std::cout << symbol << "\n";
}