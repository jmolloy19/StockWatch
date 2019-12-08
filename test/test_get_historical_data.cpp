#include <stockwatch/stock.hpp>

int main()
{
    Stock stock("AAPL");
    stock.GetHistoricalData();
    std::cout << stock;
}   