#include <stockwatch/stockwatch.hpp>

int main(int argc, char* argv[])
{
    Options options(argc, argv);
    std::string historical_data;
    
    GetHistoricalData("AAPL", &historical_data, options);
    std::cout << historical_data;
}