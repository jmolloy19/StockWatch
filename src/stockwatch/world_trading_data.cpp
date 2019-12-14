#include <stockwatch/world_trading_data.hpp>

/**
 * Makes an API call based on the requested service and writes the received data to a string.
 * @param service        enum value specifying the type of API call
 * @param buffer         string that data will be written to
 * @param stock_symbol   symbol of stock (not required for ticker api calls)
 */
void MakeApiCall(const API service, std::string* buffer, const std::string& stock_symbol)
{
    std::string url = BuildUrlForRequest(service, stock_symbol);
    MakeHttpRequest(url, buffer);
    CheckApiResponse(service, *buffer);
}

/**
 * Checks the API call response to make sure it was valid. If the response for the list of stocks
 * is invalid, the program exits. If the response for the historical data of a stock is invalid,
 * an exception is thrown. 
 * @param service        enum value specifying the type of API call
 * @param response       string that contains response from API call
 */
void CheckApiResponse(const API service, const std::string& response)
{
    if(!ValidHeader(service, response))
    {
        CheckApiKey(response);

        switch(service)
        {
            case API::HistoricalData:  
                throw std::runtime_error("Unexpected response from World Trading Data API\n");
            case API::SymbolList:
                std::cerr << "Error: Your \'stocklist.csv\' file is invalid.\n"
                          << "Check the file for an unexpected response from World Trading Data API.\n"
                          << "The number of requests for stock lists is different from normal request limit.\n"
                          << "https://www.worldtradingdata.com/documentation#stocks-indices-and-mutual-funds\n"
                          << "If you have not exceeded your limit, delete file or run with \'--update-list\'\n\n"; 
                exit(-1);
        }
    }
}

/**
 * Returns the combined url for the API call based on the requested service.
 * @param service        enum value specifying the type of API call
 * @param stock_symbol   symbol of stock (not required for ticker api calls)
 */
std::string BuildUrlForRequest(API service, const std::string& stock_symbol)
{
    const std::string url_base = "https://api.worldtradingdata.com/api/v1/";
    const std::string api_token = "&api_token=" + API_KEY;
    const std::string output = "&output=csv";
    const std::string start_date = "&date_from=" + START_DATE;
    const std::string historical_service = "history?symbol=" + stock_symbol;
    const std::string symbol_service = "ticker_list?&symbol_only=false&type=stocks";
    
    switch(service)
    {
        case API::HistoricalData:  
            return url_base + historical_service + start_date + api_token + output;
        case API::SymbolList:
            return url_base + symbol_service + api_token + output;
    } 
}

/**
 * Checks the response from the API call to see if it contains the expected header for 
 * the specified service. 
 * @param service        enum value specifying the type of API call
 * @param response       string that contains response from API call
 */
bool ValidHeader(const API service, const std::string& response)
{
    std::string expected_header;
    std::string response_start(response, 0, 50);

    switch(service)
    {
        case API::HistoricalData:  
            expected_header = "Date,Open,Close,High,Low,Volume";
            break;
        case API::SymbolList:
            expected_header = "Symbol,Name,Currency,\"Stock Exchange Long\",\"Stock Exchange Short\"";
    }

    if(response_start.find(expected_header) == std::string::npos)
    {
        return false;
    }

    return true; 
}

/**
 * Checks the response from the API call to see if it matches the message when making a 
 * call with an invalid API key.
 * @param response       string that contains response from API call
 */
void CheckApiKey(const std::string& response)
{
    std::string invalid_api_key_response = "{\"message\":\"Invalid API Key.\"}";

    if(response.find(invalid_api_key_response) != std::string::npos)
    {
        std::cerr << "World Trading Data is complaining that your API Key is invalid.\n"
				  << "Please check to make sure you inserted it correctly.\n"
                  << "Instructions and examples for inserting your API Key are in the README.md\n"
                  << "You can find your API key by logging into your account at " 
                  << "https://www.worldtradingdata.com\n\n";
		exit(-1);
    }
}