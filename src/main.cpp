#include <stockwatch/stockwatch.hpp>

int main(int argc, char* argv[])
{
    StockWatch stock_analyzer(argc, argv);

    stock_analyzer.Run();
}

