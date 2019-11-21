#include <stockwatch/stockwatch.hpp>

int main(int argc, char* argv[])
{
    bool options[MAX_OPTIONS] = {false};
    Stock stock("AAPL");
    stock.AnalyzeStock(options);
    std::cout << stock;
}