#include <stockwatch/stockwatch.hpp>

/**
 * Constructor for Stock class.
 * @param argc      command line argument count
 * @param argv[]    array of given command line arguments
 */
StockWatch::StockWatch(int argc, char* argv[]) : 
    options_(Options(argc, argv))
{
    Init();
    high_tight_flags_.reserve(num_stocks_);
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
            threads.emplace_back(&StockWatch::CheckStock, this, stocks_[thr]);    
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

    std::string stocklist_buffer;
    GetStockList(&stocklist_buffer);
    ParseStockList(stocklist_buffer);

    num_stocks_ = stocks_.size();

    std::cout << "Retrieved " << num_stocks_ << " Stocks to Analyze\n\n";
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
 * If the file './stocklist.csv' exists, it will read the stock symbol data from
 * that file and save it to the string stocklist_buffer. Else, it will make a HTTP request
 * for the list of stock symbols, save it to the string stocklist buffer, and create the
 * file './stocklist.csv' to write to. If the '--update-list' option is given, 
 * it will delete the existing './stocklist.csv' file and write a new one.
 * @param stocklist_buffer   string that data will be written to
 */
void StockWatch::GetStockList(std::string* stocklist_buffer)
{
    if(FileExists("./stocklist.csv") && !options_.UpdateList())
    {
        ReadFromFile("./stocklist.csv", stocklist_buffer);
    }
    else
    {
        std::string url = CreateUrlForRequest(API::SymbolList);
	    
        MakeHttpRequest(url, stocklist_buffer);
        CheckResponse(*stocklist_buffer);

		WriteToFile("./stocklist.csv", *stocklist_buffer);
    }
}

/**
 * Parses a string for stock symbols, and pushes each valid symbol into the vector
 * 'stocks_'. If the include NYSE program option is given, it will include stocks
 * from the NYSE. Else it will only push symbols on the NASDAQ.
 * @param stocklist_buffer 	string to parse for symbols
 */
void StockWatch::ParseStockList(const std::string& stocklist_buffer)
{
    std::string stocklist_header = "Symbol,Name,Currency,\"Stock Exchange Long\",\"Stock Exchange Short\"";


	if(stocklist_buffer.find(stocklist_header) == std::string::npos)	// No header means bad API call
	{
		std::cerr << "Error: Your \'stocklist.csv\' file is invalid.\n"
                  << "Check the file for an unexpected response from World Trading Data.\n"
                  << "To resolve issue, either delete the file or run with \'--update-list\' option.\n"; 
        exit(-1);
	}

    std::string::const_iterator it, 
						        begin = stocklist_buffer.begin(), 
						        end   = stocklist_buffer.end();
						   
	it = find(begin, end, '\n') + 1; 					         // Skip header line
	
	while(it != end)
	{
		std::string line(it, find(it, end, '\n'));               // Get a line

		std::string stock_symbol;                                       
		if(line.find("NASDAQ") != std::string::npos)             // Parse the line for a stock symbol
		{                                                        // if it's on the NASDAQ or NYSE
			stock_symbol = std::string(it, find(it, end, ','));
		}
		else if(options_.IncludeNYSE())
		{
			if(line.find("NYSE") != std::string::npos)
			{
				stock_symbol = std::string(it, find(it, end, ','));
			}
		}
		
		if(IsValidStock(stock_symbol))
		{
			stocks_.push_back(stock_symbol);
		}

		it = find(it, end, '\n') + 1;                            // Find start of next line
	}
} 

/**
 * Verifies whether the string is a valid stock symbol.
 * @param stock_symbol 	stock symbol to verify
 */
bool StockWatch::IsValidStock(const std::string& stock_symbol)
{
	if(stock_symbol.size() > 5 || stock_symbol.empty())
    {                                                            // Exclude any empty or greater
		return false;                                            // than 5 character stock_symbol  
    }					
	else
	{
		std::string::const_iterator it;
		for(it = stock_symbol.begin(); it != stock_symbol.end(); it++)
        {
			if(*it < 65 || *it > 90)                             // Exclude symbols with characters
            {                                                    // outside the range 'A' - 'Z'
				return false;
            }				                    
        }	 
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
    std::cout << "NYSE = " << options_.IncludeNYSE() << "\n";
    std::cout << "READ = " << options_.ReadFromFile() << "\n";
    std::cout << "WRITE = " << options_.WriteToFile() << "\n";
    std::cout << "UPDATE = " << options_.UpdateList() << "\n";
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