#include <StockWatch/StockWatch.hpp>
#include <iostream>

int main(int argc, char* argv[])
{
    Stock stock("AAPL");
    stock.inputData();
    std::cout << stock;
}