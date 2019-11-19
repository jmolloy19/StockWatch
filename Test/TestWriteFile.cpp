#include <StockWatch/StockWatch.hpp>

int main(int argc, char* argv[])
{
    Stock stock("AAPL");
    stock.analyze(false, true);
    std::cout << stock;
}