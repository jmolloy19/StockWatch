#include <StockWatch/StockWatch.hpp>

void analyze(const std::string& symbol)
{
    Stock stock(symbol);
    if(stock.exhibitsHTF())
        std::cout << symbol << "\n";
}