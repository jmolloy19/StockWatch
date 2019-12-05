#include <stockwatch/get_historical_data.hpp>

const std::string URL_BASE = "https://api.worldtradingdata.com/api/v1/history?symbol=";
const std::string SORT = "&sort=newest";
const std::string API_KEY_PRETEXT = "&api_token=";
const std::string OUTPUT = "&output=csv";
const std::string START_DATE_PRETEXT = "&date_from=";

/**
 * Gets the historical data of the stock. If read_from_file is true, it reads the historical data 
 * from the corresponding file in the datafiles directory. Else, it makes an API call for the data
 * @param historical_data 	string that historical data will be written to 
 * @param read_from_file  	if true, retrieves historical data from file. Else retrieves data from API call
 */
void GetHistoricalData(const std::string& stock_symbol, std::string* historical_data, bool read_from_file)
{
	if(API_KEY == "Insert_Your_API_Key_Here")
	{
		std::cout << "Please aquire an API key from https://www.worldtradingdata.com\n"
				  << "Then enter your API key in the file Stockwatch/include/stockwatch/api.hpp\n"
				  << "There are further instructions given in that file including an example.\n";
		exit(-1);
	}

	if(read_from_file)
	{
		ReadFromFile(stock_symbol + ".csv", historical_data);
	}
	else
	{
		RequestHistoricalData(stock_symbol, historical_data);
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
	CURL *curl;
	CURLcode res;
	
	std::string url_combined = URL_BASE + symbol + SORT + API_KEY_PRETEXT + API_KEY + 
							   OUTPUT + START_DATE_PRETEXT + START_DATE;

	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
	if(curl) 
	{
		historical_data_buffer->clear();
		curl_easy_setopt(curl, CURLOPT_URL, url_combined.c_str());
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

