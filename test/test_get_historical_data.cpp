#include <stockwatch/stockwatch.hpp>

int main(int argc, char* argv[])
{
    std::string historical_data;
    GetHistoricalData("AAPL", &historical_data);
    std::cout << historical_data;
}