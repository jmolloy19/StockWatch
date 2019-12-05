#include <stockwatch/stockwatch.hpp>

int main(int argc, char* argv[])
{
    Options program_options(argc, argv);
    std::vector<std::string> stock_symbols;
    
    GetStockSymbols(&stock_symbols, program_options.IncludeNYSE());

    AnalyzeStocks(stock_symbols, program_options);
}

