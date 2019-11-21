#include <stockwatch/stockwatch.hpp>

int main(int argc, char* argv[])
{
    bool options[MAX_OPTIONS] = {false};
    std::string symb(argv[1]);
    Stock stock(symb);
    stock.AnalyzeStock(options);
}