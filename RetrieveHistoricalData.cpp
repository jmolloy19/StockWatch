#define CURL_STATICLIB 
#include "C:\Users\Joseph\Documents\Stock Program\curl-7.60.0-win64-mingw\include\curl\curl.h"
#include "RetrieveHistoricalData.h"
using namespace std;

/******************************************************************************************************/
// This function is the callback function that gets called by libcurl as soon as there is data received 
// that needs to be saved. *contents points to the delivered data, nmemb is the size of the delievered data,
// size is always 1, and *userp points to where the delivered data will be written. 
/*****************************************************************************************************/
static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{	
	try
	{
		((string*)userp)->append((char*)contents, size * nmemb);
	}
	catch (bad_alloc &err)
	{
		cerr << "Memory allocation failed. Error: " << err.what();
		return 0;
	}
    return size * nmemb;
}

/*****************************************************************************************************/
// This function takes as input a string that is a stock symbol. It then uses libcurl to make a HTTP request for the files 
// containing historical data of the stock specified by the symbol. It then writes this data into a string called 
// readBuffer and returns readBuffer
/*****************************************************************************************************/
string fetchHistoricalData(string symbol)
{
	CURL *curl;
	CURLcode res;
	string readBuffer;
	string URL_combined = "https://api.iextrading.com/1.0/stock/" + symbol + "/chart/1y"; 
	//string URL_combined = "https://www.alphavantage.co/query?function=TIME_SERIES_DAILY&symbol=" + symbol + "&outputsize=compact&apikey=UHH9Y94E4GYZ0W2W&datatype=csv";
	
	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
	if(curl) 
	{
		readBuffer.clear();
		curl_easy_setopt(curl, CURLOPT_URL, URL_combined.c_str());      // Set URL to be used in HTTP request (URL_combined must first be converted to a C string)
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);	// Set WriteCallback as ptr to callback function
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);			// Set &readBuffer as ptr to where delivered data will be written
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, FALSE);			// Disable curl verifying the authenticity of a peer's SSL certificate
		res = curl_easy_perform(curl);
		if(res != CURLE_OK)
			cerr << curl_easy_strerror(res);
		else
		{
			curl_easy_cleanup(curl);
			curl_global_cleanup();
		}
	}
	return readBuffer;
}

