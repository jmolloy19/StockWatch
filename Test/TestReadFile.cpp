#include <StockWatch/StockWatch.hpp>

int main(int argc, char* argv[])
{
    Stock stock("AAPL");
    stock.analyze(true, false);
    std::cout << stock;
}