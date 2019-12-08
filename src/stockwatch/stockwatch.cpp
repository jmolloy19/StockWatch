#include <stockwatch/stockwatch.hpp>

/**
 * Constructor for Stock class.
 * @param argc      command line argument count
 * @param argv[]    array of given command line arguments
 */
StockWatch::StockWatch(int argc, char* argv[]) :
    program_options_(Options(argc, argv)),
    stocklist_file_(File("stocklist.csv", kStockListFilePath))
{}

/**
 * Analyzes every stock given by the vector 'stock_symbols_'.
 * When finished, it prints a report of all the stocks exhibiting the
 * high tight flag pattern.
 */
void StockWatch::Run()
{
    std::vector<std::thread> threads;
    threads.reserve(MAX_THREADS);

    int num_stocks = stock_symbols_.size();
    
    for(int i = 0; i < num_stocks;)
    {
        for(int thr = 0; thr < MAX_THREADS && i < num_stocks; thr++, i++)
        {
            threads.emplace_back(&StockWatch::CheckStock, this, stock_symbols_[i]);    
        }

        for(int thr = 0; thr < threads.size(); thr++)
        {
            threads[thr].join();
        }

        threads.clear();

        DisplayProgressBar(i, num_stocks);
    }

    PrintReport();
}

/**
 * Initializes the StockWatch object. Must be called before Run().
 */
void StockWatch::Init()
{
    std::string symbols_buffer;
    GetStockSymbols(&symbols_buffer);
    ParseStockSymbols(symbols_buffer);
}

/**
 * Initializes Stock object, and pushes the stock's symbol into the vector high_tight_flags_ 
 * it it exhibits the pattern.
 * @param stock_symbol  name of the stock to check
 */
void StockWatch::CheckStock(const std::string& stock_symbol)
{
    Stock stock(stock_symbol);
    stock.GetHistoricalData(program_options_.ReadFromFile(), program_options_.WriteToFile());

    if(stock.ExhibitsHighTightFlag())
    {
        high_tight_flags_.push_back(stock_symbol);
    }
}

/**
 * If the read from file option is given, it will read the stock symbol data from
 * the file './stocklist.csv'. Else is will make a HTTP request for the data.
 * The data will be written to the string 'symbols_buffer', and if the write to file
 * option is given, it will also write this string to './stocklistfile.csv'.
 * @param symbols_buffer   string that data will be written to
 */
void StockWatch::GetStockSymbols(std::string* symbols_buffer)
{
    if(program_options_.ReadFromFile())
    {
        stocklist_file_.Read(symbols_buffer);
    }
    else
    {
        CheckApiKey();
        std::string url = CreateApiCall(API::SymbolList);
	    MakeHttpRequest(url, symbols_buffer);
        CheckApiResponse(*symbols_buffer);

        if(program_options_.WriteToFile())
		{
			stocklist_file_.Write(*symbols_buffer);
		}
    }
}

/**
 * Parses a string for stock symbols, and pushes each valid symbol into the vector
 * 'stock_symbols_'. If the include NYSE program option is given, it will include stocks
 * from the NYSE. Else it will only push symbols on the NASDAQ.
 * @param symbols_buffer 	string to parse for symbols
 */
void StockWatch::ParseStockSymbols(const std::string& symbols_buffer)
{
    std::string::const_iterator it, 
						        begin = symbols_buffer.begin(), 
						        end   = symbols_buffer.end();
						   
	it = find(begin, end, '\n') + 1; 					// Skip header line
	
	while(it != end)
	{
		std::string line(it, find(it, end, '\n'));

		std::string symbol;
		if(line.find("NASDAQ") != std::string::npos)
		{
			symbol = std::string(it, find(it, end, ','));
		}
		else if(program_options_.IncludeNYSE())
		{
			if(line.find("NYSE") != std::string::npos)
			{
				symbol = std::string(it, find(it, end, ','));
			}
		}
		
		if(IsValidStockSymbol(symbol))
		{
			stock_symbols_.push_back(symbol);
		}

		it = find(it, end, '\n') + 1;
	}
} 

/**
 * Verifies whether the string is a valid stock symbol.
 * @param symbol 	stock symbol to verify
 */
bool StockWatch::IsValidStockSymbol(const std::string& symbol)
{
	if(symbol.size() > 5 || symbol.empty())					// Exclude any empty or greater than 5 character symbol
		return false;
	else
	{
		std::string::const_iterator it;
		for(it = symbol.begin(); it != symbol.end(); it++)	// Exclude symbols with characters outside range 'A' - 'Z'
			if(*it < 65 || *it > 90)				    
				return false;
		return true;
	}
}

/**
 * Prints a summarized report.
 */
void StockWatch::PrintReport()
{
    std::cout << "--Stocks That Exhibit Pattern--";
    for(int i = 0; i < high_tight_flags_.size(); i++)
    {
        if(i % 5 == 0)
        {
            std::cout << '\n';
        }
        std::cout << high_tight_flags_[i] << ", ";
    }
}

/**
 * Overloaded << operator to display StockWatch objects
 */ 
std::ostream& operator << (std::ostream& out,  const StockWatch& stocks)
{
	for(int i = 0; i < stocks.stock_symbols_.size(); i++)
	{
		out << stocks.stock_symbols_[i] << "\n";
	}
	out << "Number of Stocks to Analyze = " << stocks.stock_symbols_.size() << "\n";
    return out;
}