#include <stockwatch/get_historical_data.hpp>

/**
 * Gets the historical data of the stock. If read_from_file is true, it reads the historical data 
 * from the corresponding file in the 'datafiles' directory. Else, it makes an API call for the data
 * @param historical_data 	string that historical data will be written to 
 * @param options		  	contains the options passed through the command line
 */
void GetHistoricalData(const std::string& stock_symbol, std::string* historical_data, const Options& options)
{
	if(API_KEY == "Insert_Your_API_Key_Here")
	{
		std::cout << "Please aquire an API key from https://www.worldtradingdata.com\n"
				  << "Then enter your API key in the file Stockwatch/include/stockwatch/api.hpp\n"
				  << "There are further instructions given in that file including an example.\n";
		exit(-1);
	}

	std::string file_name = stock_symbol + ".csv";

	if(options.ReadFromFile())
	{
		ReadFromFile(file_name, kDataFilesPath, historical_data);
	}
	else
	{
		RequestHistoricalData(stock_symbol, historical_data);
	}

	if(options.WriteToFile())
	{
		WriteToFile(file_name, kDataFilesPath, *historical_data);
	}
}

/**
 * This function is the callback function that gets called by libcurl as soon as there 
 * is data received that needs to be saved.
 * @param contents 	points to delivered data 
 * @param nmemb    	the size of delievered data,
 * @param size     	is always 1
 * @param userp    	points to where delivered data will be written 
 */
static size_t RequestHistoricalDataCallback(void *contents, size_t size, size_t nmemb, void *userp)
{	
	try
	{
		((std::string*)userp)->append((char*)contents, size * nmemb);
	}
	catch (std::bad_alloc& err)
	{
		std::cerr << "Memory allocation failed. Error: " << err.what();
		return 0;
	}
    return size * nmemb;
}

/**
 * Makes a HTTP request for the file containing historical data and writes it to a string.
 * @param symbol     			 	symbol of stock who's data is being requested
 * @param historical_data_buffer 	string that historical data will be written to
 */
void RequestHistoricalData(const std::string& symbol, std::string* historical_data_buffer)
{
	std::string request_url = CreateRequestURL(API::HistoricalData, symbol);
	
	CURL *curl;
	CURLcode res;

	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
	if(curl) 
	{
		historical_data_buffer->clear();
		curl_easy_setopt(curl, CURLOPT_URL, request_url.c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, RequestHistoricalDataCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, historical_data_buffer);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
		res = curl_easy_perform(curl);
		if(res != CURLE_OK)
			std::cerr << curl_easy_strerror(res);
		else
		{
			curl_easy_cleanup(curl);
			curl_global_cleanup();
		}
	}
}

