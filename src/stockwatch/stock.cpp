#include <stockwatch/stock.hpp>

/**
 * Constructor for Stock class. 
 */
Stock::Stock(const std::string& symbol) : stock_name_(symbol), number_of_days_(0) {}

/**
 * Parses historical data for closing prices and volumes of each trading day.
 * Data is pushed in the order of most recent trading day first to oldest trading day last.
 * @param historical_data 	string of historical data that will be parsed and inputted into object
 * @param write_to_file   	if true, writes historical data to file in stockdatafiles directory
 */
void Stock::InputHistoricalData(const std::string& historical_data)
{
    std::string csv_header = "Date,Open,Close,High,Low,Volume";

	if(historical_data.find(csv_header) == -1)
	{
		std::cerr << "Unexpected response from World Trading Data API: " << stock_name_ << ": " << historical_data << "\n";
	}
	else	
    {                                                        															       
		std::string::const_iterator it,
							        begin = historical_data.begin(), 
							        end = historical_data.end();
		
		size_t found = 32;							                 // Skip header	    
	
        while(it != end)						                
		{	
            for(int i = 0; i < 2; i++)                               // Sets 'found' to comma before close value
            {
                found = historical_data.find(',', found + 1);  
            }
			if (found != -1)						              	 // Check we're not past last trading day entry
			{
				number_of_days_++;							    
				it = begin + found + 1; 					      	 // Sets 'it' to start of close value
				std::string closeStr(it, std::find(it, end, ','));	 // Extract close value
				closes_.push_back(std::stod(closeStr));

                for(int i = 0; i < 3; i++)                           // Set 'found' to comma before volume value
                {
                    found = historical_data.find(',', found + 1);  
                }                  
                it = begin + found + 1; 					      	 // Sets 'it' to start of volume value
				std::string volumeStr(it, std::find(it, end, '\n')); // Extract volume value
				volumes_.push_back(std::stoi(volumeStr));
                it = std::find(it, end, ',');
			}
		}
	}
}

/**
 * Gets the historical data for the stock, and inputs the data into the object.
 * It then prints the name of the stock if it exhibits any patterns.
 * @param options	options passed through the command line
 */
void Stock::Analyze(const Options& options)
{
	std::string historical_data;

	GetHistoricalData(stock_name_, &historical_data, options);
	InputHistoricalData(historical_data);
    if(ExhibitsHighTightFlag())
	{
        std::cout << stock_name_ << "\n";
	}
}

/**
 * Returns true if a stock exhibits the high and tight flag pattern.
 */
bool Stock::ExhibitsHighTightFlag()
{
	if(number_of_days_ < 60 )
	{
		return false;
	}													
		
	std::vector<double>::iterator it,
							      most_recent = closes_.begin(),
							      lowest = min_element(most_recent , most_recent + 60),
								  highest = max_element(most_recent, lowest);
	if(*lowest == 0.0)
	{
		return false;
	}
	else if (*highest / *lowest < 1.9)
	{
		return false;
	}
	else if( std::distance(most_recent, highest) > 15 )
	{
		return false;
	}
	else
	{
		for(it = most_recent; it < highest; it++)
		{
			if( *it < (*highest * .8) )
			{
				return false;
			}
		}	
	}
			
	return true;
}

/**
 * Overloaded << operator to display Stock objects
 */ 
std::ostream& operator << (std::ostream& out, const Stock& stock)
{
	out << "Stock = " << stock.stock_name_ << "\n";
	out << "#\tCloses\tVolume\n";
	for(int i = 0; i < stock.number_of_days_; i++)
	{
		out << i << "\t" << stock.closes_[i] << "\t" << stock.volumes_[i] << "\n";
	}
    return out;
}