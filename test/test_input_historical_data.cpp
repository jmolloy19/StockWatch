#include <stockwatch/stockwatch.hpp>

int main(int argc, char* argv[])
{
    Options options;
    Stock stock("AAPL");
    
    stock.Analyze(options);
    std::cout << stock;
}