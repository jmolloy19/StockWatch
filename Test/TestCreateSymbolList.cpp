#include <StockWatch/StockWatch.hpp>
#include <iostream>

int main(int argc, char* argv[])
{
    std::vector<std::string> symbols;
    createSymbolList(&symbols);

    std::cout << "# of symbols aquired: " << symbols.size() << "\n"; 
    for(int i = 0; i < symbols.size(); i++)
    {
        std::cout << symbols[i] << "\n";
    }
}