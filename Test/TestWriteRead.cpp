#include <StockWatch/StockWatch.hpp>
#include <iostream>

int main(int argc, char* argv[])
{
    Stock stock("AAPL");
    stock.analyze(false, true);
    stock.analyze(true, false);
    std::cout << stock;
}