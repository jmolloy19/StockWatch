#include <stockwatch/stockwatch.hpp>

int main(int argc, char* argv[])
{
    Options options;
    options.Set(false, false, true);
    Stock stock("AAPL");
    
    stock.Analyze(options);
}