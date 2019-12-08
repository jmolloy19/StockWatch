#include <stockwatch/stockwatch.hpp>

const char* kStockListFilePath = "./stocklist.csv";

/**
 * Constructor for Stock class.
 * @param argc      command line argument count
 * @param argv[]    array of given command line arguments
 */
StockWatch::StockWatch(int argc, char* argv[]) : 
    options_(Options(argc, argv))
{
    Init();
}

/**
 * Analyzes every stock given by the vector 'stocks_'.
 * When finished, it prints a report of all the stocks exhibiting the
 * high tight flag pattern.
 */
void StockWatch::Run()
{
    std::vector<std::thread> threads;
    threads.reserve(MAX_THREADS);
    
    int i = 0;
    while(i < num_stocks_)
    {
        for(int thr = 0; thr < MAX_THREADS && i < num_stocks_; thr++, i++)
        {
            threads.emplace_back(&StockWatch::CheckStock, this, stocks_[i + thr]);    
        }

        for(int thr = 0; thr < threads.size(); thr++)
        {
            threads[thr].join();
        }

        threads.clear();
        DisplayProgressBar(i, num_stocks_);
    }

    PrintReport();
}

/**
 * Initializes StockWatch by creating the list of stocks to analyze.
 */
void StockWatch::Init()
{
    std::cout << "\nAqcuiring List of Stocks . . ." << "\r" << std::flush;

    std::string symbols_buffer;
    GetStockList(&symbols_buffer);
    ParseStockList(symbols_buffer);

    num_stocks_ = stocks_.size();

    std::cout << "Retrieved " << stocks_.size() << " Stocks to Analyze\n\n";
}

/**
 * Checks the stock given by stock_symbol, and pushes it into the vector high_tight_flags_
 * if the stock exhibits the pattern.
 * @param stock_symbol  name of the stock to check
 */
void StockWatch::CheckStock(const std::string& stock_symbol)
{
    Stock stock(stock_symbol, options_.ReadFromFile(), options_.WriteToFile());

    if(stock.ExhibitsHighTightFlag())
    {
        std::unique_lock<std::mutex> lock(mtx_);
        high_tight_flags_.push_back(stock_symbol);
        lock.unlock();
    }
}

/**
 * If the read from file option is given, it will read the stock symbol data from
 * the file './stocklist.csv'. Else is will make a HTTP request for the data.
 * The data will be written to the string 'symbols_buffer', and if the write to file
 * option is given, it will also write this string to './stocklistfile.csv'.
 * @param symbols_buffer   string that data will be written to
 */
void StockWatch::GetStockList(std::string* symbols_buffer)
{
    if(options_.ReadFromFile())
    {
        ReadFromFile(kStockListFilePath ,symbols_buffer);
    }
    else
    {
        CheckApiKey();
        std::string url = CreateUrl(API::SymbolList);
	    MakeHttpRequest(url, symbols_buffer);
        CheckApiResponse(*symbols_buffer);

        if(options_.WriteToFile())
		{
			WriteToFile(kStockListFilePath, *symbols_buffer);
		}
    }
}

/**
 * Parses a string for stock symbols, and pushes each valid symbol into the vector
 * 'stocks_'. If the include NYSE program option is given, it will include stocks
 * from the NYSE. Else it will only push symbols on the NASDAQ.
 * @param symbols_buffer 	string to parse for symbols
 */
void StockWatch::ParseStockList(const std::string& symbols_buffer)
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
		else if(options_.IncludeNYSE())
		{
			if(line.find("NYSE") != std::string::npos)
			{
				symbol = std::string(it, find(it, end, ','));
			}
		}
		
		if(IsValidStock(symbol))
		{
			stocks_.push_back(symbol);
		}

		it = find(it, end, '\n') + 1;
	}
} 

/**
 * Verifies whether the string is a valid stock symbol.
 * @param symbol 	stock symbol to verify
 */
bool StockWatch::IsValidStock(const std::string& symbol)
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
 * Prints out list of stocks that StockWatch will analyze
 */
void StockWatch::PrintStockList()
{
    for(int i = 0; i < num_stocks_; i++)
	{
		std::cout << stocks_[i] << "\n";
	}
    std::cout << "\nNumber of Stocks = " << stocks_.size() << "\n";
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
