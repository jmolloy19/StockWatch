#include <stockwatch/stockwatch.hpp>

int main(int argc, char* argv[])
{
    Stock stock("AAPL", true, false);
    std::cout << stock;
}   