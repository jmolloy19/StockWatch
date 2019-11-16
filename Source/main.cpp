#include <StockWatch/StockWatch.hpp>
#include <Utility/Utility.hpp>

int main(int argc, char* argv[])
{
    std::vector<std::thread> threads;
    std::vector<Stock> stocks;
    std::vector<std::string> symbols;
    bool options[MAX_OPTIONS] = {false};

    threads.reserve(THREAD_COUNT);
    stocks.reserve(THREAD_COUNT);

    checkCmdLineOptions(argc, argv, options);

    createSymbolList(&symbols, options[0]);

    int numStocks = symbols.size();
    int i = 0;
    while(i < numStocks)
    {
        for(int thr = 0; thr < THREAD_COUNT && i < numStocks; thr++, i++)
        {
            stocks.emplace_back(symbols[i]);
            threads.emplace_back(&Stock::analyze, std::ref(stocks[thr]), std::ref(options[1]), std::ref(options[2]));    
        }
        for(int thr = 0; thr < threads.size(); thr++)
        {
            threads[thr].join();
        }
        threads.clear();
        stocks.clear();
        if(i % 100 == 0)
            std::cout << i << " Stocks Done\n" << std::flush;
    }
    
}