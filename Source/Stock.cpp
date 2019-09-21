#include <StockWatch/Stock.hpp>
#include <StockWatch/HistoricalData.hpp>

/*****************************************************************************************************/
// Constructor for Stock class 
/*****************************************************************************************************/ 
Stock::Stock(const std::string& symbol) : name_(symbol), gotValidData_(false), numDays_(0)
{
    std::string historicalData;
    fetchHistoricalData(symbol, &historicalData);
    inputData(historicalData);
}

/*****************************************************************************************************/
// This function takes as input the string returned by fetchHistoricalData(). It parses the string for 
// the closing prices and daily volumes_ of each trading day and inputs the data into a HistoricalData object. 
// Data is inputted in the order of most recent trading day first to oldest trading day last
/*****************************************************************************************************/  
void Stock::inputData(const std::string& historicalData)
{
    std::string csvHeader = "Date,Open,Close,High,Low,Volume";

	if(historicalData.find(csvHeader) != -1) 		        // If string does not contain the header string, the HTTP request was most likely invalid	
    {                                                        															       
		std::string::const_iterator it,
							        begin = historicalData.begin(), 
							        end = historicalData.end();
		
		size_t found = 32;							        // Initial found to position right after header	    
	
        while(it != end)						                
		{	
            for(int i = 0; i < 2; i++)                      // This loop sets found to position of comma before close value
            {
                found = historicalData.find(',', found + 1);  
            }
			if (found != -1)						            // Check to make sure we're not past last trading day entry
			{
				numDays_++;							    
				it = begin + found + 1; 					    // Sets it to position of start of close value
				std::string closeStr(it, find(it, end, ','));	// Extract close value
				closes_.push_back(std::stod(closeStr));

                for(int i = 0; i < 3; i++)                      // This loop sets found to position of comma before volume value
                {
                    found = historicalData.find(',', found + 1);  
                }                  
                it = begin + found + 1; 					    // Sets it to position of start of volume value
				std::string volumeStr(it, find(it, end, '\n'));	// Extract volume value
				volumes_.push_back(std::stoi(volumeStr));
                it = find(it, end, ',');
			}
		}
	}
}

/*****************************************************************************************************/
// This function returns the size of the HistoricalData object (or number of trading days of data saved to object)
/*****************************************************************************************************/ 
int Stock::getNumDays() const
{
	return numDays_;
}

/*****************************************************************************************************/
// This function returns true if the historical data of a stock exhibits patterns of a high and tight flag.
// Otherwise it returns false
/*****************************************************************************************************/  
bool Stock::checkForHTF()
{
	if( !gotValidData_ || numDays_ < 60 )													
		return false;
		
	std::vector<double>::iterator it,
							      mostRecent = closes_.begin(),
							      highest = max_element(mostRecent, mostRecent + 60),
							      lowest = min_element(mostRecent , mostRecent + 60);
	
	if(*lowest == 0.0)
		return false;
	else if (*highest / *lowest < 2.0)
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

/*****************************************************************************************************/
// Overloaded << operator to display HistoricalData objects
/*****************************************************************************************************/ 
std::ostream& operator << (std::ostream& out, const Stock& stock)
{
	out << "#\tCloses\tVolume\n";
	for(int i = 0; i < stock.numDays_; i++)
		out << i << "\t" << stock.closes_[i] << "\t" << stock.volumes_[i] << "\n";
    return out;
}