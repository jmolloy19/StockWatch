#include <StockWatch/StockWatch.hpp>
#include <iostream>

int main(int argc, char* argv[])
{
    Stock stock("AAPL");
    stock.analyze(false, true);
    std::cout << stock;
}