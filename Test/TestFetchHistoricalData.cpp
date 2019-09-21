#include <StockWatch/StockWatch.hpp>
#include <iostream>

int main(int argc, char* argv[])
{
    std::string historicalData;
    fetchHistoricalData("AAPL", &historicalData);
    std::cout << historicalData;
}