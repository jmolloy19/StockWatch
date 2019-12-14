#include <stockwatch/world_trading_api.hpp>

/**
 * Returns the combined url for the API call based on the requested service.
 * @param service        enum value specifying the type of API call
 * @param stock_symbol   symbol of stock (not required for ticker api calls)
 */
std::string CreateUrlForRequest(API service, const std::string& stock_symbol)
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

void CheckResponse(const std::string& response)
{
    std::string invalid_api_key_response = "{\"message\":\"Invalid API Key.\"}";
    std::string beginning(response, 0, 40);

    CheckApiKeyChanged();

    if(beginning.find(invalid_api_key_response) != std::string::npos)
    {
        std::cerr << "World Trading Data is complaining that your API Key is invalid.\n"
				  << "Please check to make sure you inserted it correctly.\n"
                  << "Instructions and examples for inserting your API Key are in the README.md\n"
                  << "You can find your API key by logging into your account at " 
                  << "https://www.worldtradingdata.com\n\n";
		exit(-1);
    }
}

void CheckApiKeyChanged()
{
    if(API_KEY == "Insert_Your_API_Key_Here")
	{
		std::cerr << "Please aquire an API key from https://www.worldtradingdata.com\n"
				  << "Then enter your API key in the file Stockwatch/include/stockwatch/world_trading_api.hpp\n"
				  << "Instructions and examples for inserting your API Key are in the README.md\n\n";
		exit(-1);
	}
}