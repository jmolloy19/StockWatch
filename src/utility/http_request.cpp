#include <utility/http_request.hpp>

/**
 * This function is the callback function that gets called by libcurl as soon as there 
 * is data received that needs to be saved.
 * @param contents 	points to delivered data 
 * @param nmemb    	the size of delievered data,
 * @param size     	is always 1
 * @param userp    	points to where delivered data will be written 
 */
static size_t Callback(void* contents, size_t size, size_t nmemb, void* userp)
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
 * Makes HTTP request for the file containing historical data and writes it to a string.
 * @param url     	url to be used for HTTP request
 * @param buffer 	string that received data will be written to
 */
void MakeHttpRequest(const std::string& url, std::string* buffer)
{	
	CURL *curl;
	CURLcode res;

	curl = curl_easy_init();
	if(curl) 
	{
		buffer->clear();
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, Callback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, buffer);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
		res = curl_easy_perform(curl);
		if(res != CURLE_OK)
			std::cerr << curl_easy_strerror(res);
		else
		{
			curl_easy_cleanup(curl);
		}
	}
}