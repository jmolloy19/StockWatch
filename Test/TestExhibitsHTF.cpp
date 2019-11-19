#include <StockWatch/StockWatch.hpp>
#include <iostream>

int main(int argc, char* argv[])
{
    std::string symb(argv[1]);
    Stock stock(symb);
    stock.analyze(false, false);
    //std::cout << stock;
}