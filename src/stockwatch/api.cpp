#include <stockwatch/api.hpp>

/**
 * Returns the combined url for the API call based on the requested service.
 * @param service        enum value specifying the type of API call
 * @param stock_symbol   symbol of stock (not required for ticker api calls)
 */
std::string CreateRequestURL(API service, const std::string& stock_symbol)
{
    const std::string url_base = "https://api.worldtradingdata.com/api/v1/";
    const std::string api_token = "&output=csv&api_token=" + API_KEY;
    
    if(service == API::HistoricalData)
    {
        const std::string history = "history?symbol=" + stock_symbol;
        const std::string date = "&date_from=" + START_DATE;
        
        return url_base + history + date + api_token;
    }
    else
    {
        const std::string ticker = "ticker_list?&symbol_only=false";
        
        return url_base + ticker + api_token;
    }  
}