#include <StockWatch/StockWatch.hpp>

void analyze(const std::string& symbol)
{
    Stock stock(symbol);
    stock.inputData();
    if(stock.exhibitsHTF())
        std::cout << symbol << "\n";
}