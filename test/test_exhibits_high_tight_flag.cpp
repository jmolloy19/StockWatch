#include <stockwatch/stockwatch.hpp>

int main(int argc, char* argv[])
{
    Options options;
    if(argc != 2)
    {
        std::cout << "Please enter a stock symbol as the only argument (no options)\n";
        exit(-1);
    }
    std::string symb(argv[1]);
    Stock stock(symb);
    stock.Analyze(options);
}