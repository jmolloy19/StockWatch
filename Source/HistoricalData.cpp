//#define CURL_STATICLIB 
#include <curl/curl.h>
#include <iostream>
#include <StockWatch/HistoricalData.hpp>
#include <StockWatch/Api.hpp>


/**
 * This function is the callback function that gets called by libcurl as soon as there is data received 
 * that needs to be saved. *contents points to the delivered data, nmemb is the size of the delievered data,
 * size is always 1, and *userp points to where the delivered data will be written. 
 */
static size_t writeDataCallback(void *contents, size_t size, size_t nmemb, void *userp)
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
 * This function takes as input a string that is a stock symbol. It then uses libcurl to make a HTTP request for the files 
 * containing historical data of the stock specified by the symbol. It then writes this data into the string
 * readBuffer
 */
void fetchHistoricalData(const std::string& symbol, std::string* readBuffer)
{
	CURL *curl;
	CURLcode res;
	
	std::string urlCombined =  urlBase + symbol + sort + apiKey + output + startDate;

	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
	if(curl) 
	{
		readBuffer->clear();
		curl_easy_setopt(curl, CURLOPT_URL, urlCombined.c_str());      		// Set URL to be used in HTTP request (must first be converted to a C string)
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeDataCallback);	// Set WriteCallback as ptr to callback function
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, readBuffer);				// Set &readBuffer as ptr to where delivered data will be written
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);				// Disable curl verifying the authenticity of a peer's SSL certificate
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

