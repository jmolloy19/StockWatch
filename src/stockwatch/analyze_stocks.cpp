#include <stockwatch/analyze_stocks.hpp>

/**
 * Analyzes every stock given by the vector 'stock_symbols'.
 * @param stock_symbols	    vector containing the symbols of stocks to be analyzed
 * @param options	        program options passed through the command line
 */
void AnalyzeStocks(const std::vector<std::string>& stock_symbols, const Options& options)
{
    std::vector<Stock> stocks;
    std::vector<std::thread> threads;

    stocks.reserve(MAX_THREADS);
    threads.reserve(MAX_THREADS);

    int num_stocks = stock_symbols.size();
    
    for(int i = 0; i < num_stocks;)
    {
        for(int thr = 0; thr < MAX_THREADS && i < num_stocks; thr++, i++)
        {
            stocks.emplace_back(stock_symbols[i]);
            threads.emplace_back(&Stock::Analyze, &stocks[thr], options);    
        }

        for(int thr = 0; thr < threads.size(); thr++)
        {
            threads[thr].join();
        }

        stocks.clear();
        threads.clear();

        DisplayProgressBar(i, num_stocks);
    }
}

void PrintReport()
{
    Stock report("AAPL");
    //std::vector<std::string> high_tight_flags;
    std::cout << "Stocks That Exhibit Pattern";
    for(int i = 0; i < report.high_tight_flags_.size(); i++)
    {
        if(i % 5 == 0)
        {
            std::cout << '\n';
        }
        std::cout << report.high_tight_flags_[i];
    }
}