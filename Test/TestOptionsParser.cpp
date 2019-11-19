#include <StockWatch/StockWatch.hpp>

int main(int argc, char* argv[])
{
    bool options[MAX_OPTIONS] = {false};
    checkCmdLineOptions(argc, argv, options);
    std::cout << "Option NYSE: "  << options[0] << "\n";
    std::cout << "Option Read: "  << options[1] << "\n";
    std::cout << "Option Write: "  << options[2] << "\n";
}