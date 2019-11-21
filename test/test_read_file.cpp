#include <stockwatch/stockwatch.hpp>

int main(int argc, char* argv[])
{
    bool options[MAX_OPTIONS] = {false, true, false};
    Stock stock("AAPL");
    stock.AnalyzeStock(options);
    std::cout << stock;
}