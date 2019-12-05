#include <stockwatch/stockwatch.hpp>

int main(int argc, char* argv[])
{
    Options options;
    options.Set(false, true, false);
    Stock stock("AAPL");
    
    stock.Analyze(options);
    std::cout << stock;
}