#include <StockWatch/StockWatch.hpp>

int main(int argc, char* argv[])
{
    std::vector<std::string> symbols;

    createSymbolList(&symbols, false);
    for(int i = 0; i < symbols.size(); i++)
    {
        std::cout << symbols[i] << "\n";
    }

    symbols.clear();
    std::cout << "\nNASDAQ + NYSE:\n";
    createSymbolList(&symbols, true);

    symbols.clear();
    std::cout << "NASDAQ:\n";
    createSymbolList(&symbols, false);
}