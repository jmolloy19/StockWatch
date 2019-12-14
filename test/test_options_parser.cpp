#include <stockwatch/stockwatch.hpp>

int main(int argc, char* argv[])
{
    Options opt(argc, argv);
    std::cout << "NYSE = " << opt.IncludeNYSE() << "\n";
    std::cout << "READ = " << opt.ReadFromFile() << "\n";
    std::cout << "WRITE = " << opt.WriteToFile() << "\n";
    std::cout << "UPDATE = " << opt.UpdateList() << "\n";
}   