#include <stockwatch/stockwatch.hpp>
#include <utility/utility.hpp>
#include <thread>

int main(int argc, char* argv[])
{
    
    bool cmd_line_options[MAX_OPTIONS] = {false};
    ParseCmdLineOptions(argc, argv, cmd_line_options);

    std::vector<std::string> stock_symbols;
    GetStockSymbols(&stock_symbols, cmd_line_options[0]);

    std::vector<Stock> stocks;
    std::vector<std::thread> threads;
    stocks.reserve(MAX_THREADS);
    threads.reserve(MAX_THREADS);

    int number_of_stocks = stock_symbols.size();
    int i = 0;
    
    while(i < number_of_stocks)
    {
        for(int thr = 0; thr < MAX_THREADS && i < number_of_stocks; thr++, i++)
        {
            stocks.emplace_back(stock_symbols[i]);
            threads.emplace_back(&Stock::AnalyzeStock, &stocks[thr], cmd_line_options);    
        }
        for(int thr = 0; thr < threads.size(); thr++)
        {
            threads[thr].join();
        }

        stocks.clear();
        threads.clear();

        if(i % 100 == 0)
            std::cout << i << " Stocks Done\n" << std::flush;
    }
    
}