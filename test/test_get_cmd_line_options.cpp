#include <stockwatch/stockwatch.hpp>

int main(int argc, char* argv[])
{
    Options options(argc, argv);
    
    std::cout << "Option NYSE: "  << options.IncludeNYSE() << "\n";
    std::cout << "Option Read: "  << options.ReadFromFile() << "\n";
    std::cout << "Option Write: "  << options.WriteToFile() << "\n";
}