#include <StockWatch/Stock.hpp>

/**
 * Constructor for Stock class. 
 */
Stock::Stock(const std::string& symbol) : 
	name_(symbol), 
	gotValidData_(false), 
	numDays_(0)
{}

/**
 * Parses historical data for closing prices and volumes of each trading day.
 * Data is pushed in the order of most recent trading day first to oldest trading day last.
 * @param read  if true, retrieves historical data from files. Else retrieves data from API calls
 * @param write if true, writes historical data to files
 */
void Stock::inputData(bool read, bool write)
{
	std::string historicalData;

	if(read)
	{
		readFromFile(name_ + ".csv", &historicalData);	
	}
	else
	{	
    	fetchHistoricalData(name_, &historicalData);
	}

	if(write)
	{
		writeToFile(name_ + ".csv", historicalData);	
	}
	
    std::string csvHeader = "Date,Open,Close,High,Low,Volume";

	if(historicalData.find(csvHeader) == -1)
	{
		std::cerr << "Unexpected response from World Trading Data API: " << name_ << ": " << historicalData << "\n";
	}
	else	
    {                                                        															       
		std::string::const_iterator it,
							        begin = historicalData.begin(), 
							        end = historicalData.end();
		
		size_t found = 32;							                 // Skip header	    
	
        while(it != end)						                
		{	
            for(int i = 0; i < 2; i++)                               // Sets 'found' to position of comma before close value
            {
                found = historicalData.find(',', found + 1);  
            }
			if (found != -1)						              	 // Check to make sure we're not past last trading day entry
			{
				numDays_++;							    
				it = begin + found + 1; 					      	 // Sets 'it' to position of start of close value
				std::string closeStr(it, std::find(it, end, ','));	 // Extract close value
				closes_.push_back(std::stod(closeStr));

                for(int i = 0; i < 3; i++)                           // Set 'found' to position of comma before volume value
                {
                    found = historicalData.find(',', found + 1);  
                }                  
                it = begin + found + 1; 					      	 // Sets 'it' to position of start of volume value
				std::string volumeStr(it, std::find(it, end, '\n')); // Extract volume value
				volumes_.push_back(std::stoi(volumeStr));
                it = std::find(it, end, ',');
			}
		}
	}
}

/**
 * Analyzes stock data and prints name if it exhibits pattern.
 * @param options pointer to array specifying given command line options
 */
void Stock::analyze(bool read, bool write)
{
	inputData(read, write);
    if(exhibitsHTF())
        std::cout << name_ << "\n";
}

/**
 * Returns true if a stock exhibits the high and tight flag pattern.
 */
bool Stock::exhibitsHTF()
{
	if( !gotValidData_ || numDays_ < 60 )													
		return false;
		
	std::vector<double>::iterator it,
							      mostRecent = closes_.begin(),
							      highest = max_element(mostRecent, mostRecent + 60),
							      lowest = min_element(mostRecent , mostRecent + 60);
	
	if(*lowest == 0.0)
		return false;
	else if (*highest / *lowest < 1.9)
		return false;
	else if(highest > lowest)			
		return false;
	else if( std::distance(mostRecent, highest) > 15 )
		return false;
	else
		for(it = mostRecent; it <= mostRecent + 15; it++)
			if( *it < (*highest * .8) )
				return false;
			
	return true;
}

/**
 * Overloaded << operator to display Stock objects
 */ 
std::ostream& operator << (std::ostream& out, const Stock& stock)
{
	out << "Stock = " << stock.name_ << "\n";
	out << "#\tCloses\tVolume\n";
	for(int i = 0; i < stock.numDays_; i++)
		out << i << "\t" << stock.closes_[i] << "\t" << stock.volumes_[i] << "\n";
    return out;
}