#include <stockwatch/stockwatch.hpp>

int main(int argc, char* argv[])
{
    StockWatch analyzer(argc, argv);
    analyzer.Init();
    std::cout << analyzer;
}   