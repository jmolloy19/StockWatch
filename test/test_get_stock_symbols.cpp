#include <stockwatch/stockwatch.hpp>

int main(int argc, char* argv[])
{
    std::vector<std::string> symbols;

    GetStockSymbols(&symbols, false);
    for(int i = 0; i < symbols.size(); i++)
    {
        std::cout << symbols[i] << "\n";
    }

    symbols.clear();
    std::cout << "\nNASDAQ + NYSE:\n";
    GetStockSymbols(&symbols, true);

    symbols.clear();
    std::cout << "NASDAQ:\n";
    GetStockSymbols(&symbols, false);
}