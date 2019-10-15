#include <curl/curl.h>
#include <iostream>
#include <algorithm>
#include <StockWatch/StockSymbols.hpp>


/**
 * This function is the callback function that gets called by libcurl as soon as there is data received 
 * that needs to be saved. For its arguments, *contents points to the delivered data, nmemb is the size of the
 * delivered data, size is always 1, and *userp points to where the delivered data will be written.
 */
static size_t writeSymbolsCallback(void *contents, size_t size, size_t nmemb, void *userp)
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
 * This function uses libcurl to make HTTP requests for files containing the names and symbols of all stocks
 * listed on the NASDAQ and NYSE. It then writes the data from these files into the passed string (readBuffer)
 */
void fetchStockSymbols(std::string* readBuffer)
{
	CURL *curl;
	CURLcode res;
	
	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
	if(curl) 
    {
	  	readBuffer->clear();
	    curl_easy_setopt(curl, CURLOPT_URL, "ftp://ftp.nasdaqtrader.com/symboldirectory/nasdaqlisted.txt");   // Set URL to be used in HTTP request (list of NASDAQ stocks)
	    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeSymbolsCallback);								  // Set writeSymbolsCallback as ptr to callback function
	    curl_easy_setopt(curl, CURLOPT_WRITEDATA, readBuffer);												  // Set readBuffer as ptr to where delivered data will be written
	    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);												  // Disable curl verifying the authenticity of a peer's SSL certificate
	    res = curl_easy_perform(curl);
	    if(res != CURLE_OK)
	    	std::cerr << curl_easy_strerror(res);
	    else
	    {
	    	curl_easy_setopt(curl, CURLOPT_URL, "ftp://ftp.nasdaqtrader.com/symboldirectory/otherlisted.txt"); // Change URL to be used in second request (List of NYSE stocks)
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
}

/**
 * This function calls fetchStockSymbols(). It then parses the string for valid
 * stock symbols, and pushes each symbol into passed vector (stockSymbols)
 */
void createSymbolList(std::vector<std::string>* stockSymbols)
{
    std::string readBuffer;
    fetchStockSymbols(&readBuffer);

	std::string::const_iterator it, 
						        begin = readBuffer.begin(), 
						        end = readBuffer.end();
						   
	it = find(begin, end, '\n') + 1; 					// + 1 for position of character after '\n' (it points to first character in second line)
	
	while(it != end)									// Loops through readBuffer until end is reached
	{
		std::string symbol(it, find(it, end, '|'));		// Parses line for stock symbol
		if(checkValidSymbol(symbol))					// Checks if symbol is valid, and if so, pushes it onto the vector stockSymbols
			stockSymbols->push_back(symbol);
		it = find(it, end, '\n') + 1;
	}
}

/**
 * This function takes as input a string and verifies whether the string is a valid stock symbol
 */
bool checkValidSymbol(const std::string& symbol)
{
	if(symbol.size() > 5)									// Any symbol greater than 5 characters is probably not a valid stock symbol
		return false;
	else
	{
		std::string::const_iterator it;
		for(it = symbol.begin(); it != symbol.end(); it++)	// Any symbol containing characters outside of the range 'A' - 'Z'
			if(*it < 65 || *it > 90)					    // is probably not a valid stock symbol (or not what I'm looking to analyze)
				return false;
		return true;
	}
}
