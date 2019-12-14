#include <stockwatch/stock.hpp>

/**
 * Constructor for Stock class.
 * @param symbol  the symbol of the stock 
 */
Stock::Stock(const std::string& symbol, bool read_file, bool write_file) : 
	symbol_(symbol), 
	read_file_(read_file),
	write_file_(write_file)
{
	Init();
}

/**
 * Returns true if a stock exhibits the high and tight flag pattern.
 */
bool Stock::ExhibitsHighTightFlag()
{
	if(num_trading_days_ < 60 )
	{
		return false;
	}													
		
	std::vector<double>::iterator it,
							      most_recent = closes_.begin(),
							      lowest 	  = min_element(most_recent , most_recent + 60),
								  highest 	  = max_element(most_recent, lowest);
	if(*lowest == 0.0)
	{
		return false;
	}
	if (*highest / *lowest < 1.9)
	{
		return false;
	}
	if( std::distance(most_recent, highest) > 15 )
	{
		return false;
	}
	
	for(it = most_recent; it < highest; it++)
	{
		if( *it < (*highest * .8) )
		{
			return false;
		}
	}	
				
	return true;
}

/**
 * Initializes Stock object.
 */
void Stock::Init()
{
	std::string data_buffer;
	try
	{
		GetHistoricalData(&data_buffer);	
		ParseHistoricalData(data_buffer);
	}
	catch(const std::exception& e)
	{
		return;
	}
}

/**
 * Gets the historical data of the stock. If read_file is true, it reads the historical data 
 * from the corresponding file in the 'datafiles' directory. Else, it makes an API call for the data
 * @param read_from_file 	if true, reads historical data from file 
 * @param write_to_file		if true, writes historical data to file
 */
void Stock::GetHistoricalData(std::string* data_buffer)
{
	std::string file_path = "./datafiles/" + symbol_ + ".csv";
	
	if(read_file_)
	{
		ReadFromFile(file_path, data_buffer);
	}
	else
	{
		MakeApiCall(API::HistoricalData, data_buffer, symbol_);

		if(write_file_)
		{
			WriteToFile(file_path, *data_buffer);
		}
	}
}

/**
 * Parses historical data for closing prices and volumes of each trading day.
 * Data is pushed in the order of most recent trading day first to oldest trading day last.
 * @param historical_data 	string of historical data that will be parsed and inputted into object
 */
void Stock::ParseHistoricalData(const std::string& historical_data)
{                                             															       
	std::string::const_iterator it,
								begin = historical_data.begin(), 
								end = historical_data.end();
	
	size_t found = 32;							                     // Skip header	    

	while(it != end)						                
	{	
		for(int i = 0; i < 2; i++)                               // Find start of close value
		{
			found = historical_data.find(',', found + 1);  
		}
		if (found != std::string::npos)						     // Check we're not past last trading day entry
		{
			num_trading_days_++;							    
			it = begin + found + 1; 					      	 // Sets 'it' to start of close value
			std::string closeStr(it, std::find(it, end, ','));	 // Extract close value
			closes_.push_back(std::stod(closeStr));

			for(int i = 0; i < 3; i++)                           // Find start of volume value
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

/**
 * Overloaded << operator to display Stock objects
 */ 
std::ostream& operator << (std::ostream& out, const Stock& stock)
{
	out << "Stock = " << stock.symbol_ << "\n";
	out << "#\tCloses\tVolume\n";
	for(int i = 0; i < stock.num_trading_days_; i++)
	{
		out << i << "\t" << stock.closes_[i] << "\t" << stock.volumes_[i] << "\n";
	}
    return out;
}