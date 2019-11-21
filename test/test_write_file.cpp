#include <stockwatch/stockwatch.hpp>

int main(int argc, char* argv[])
{
    bool options[MAX_OPTIONS] = {false, false, true};
    Stock stock("AAPL");
    stock.AnalyzeStock(options);
}