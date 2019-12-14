#pragma once
#include <string>
#include <iostream>

/**
 * World Trading Data API
 * Documention: https://www.worldtradingdata.com/documentation#full-history
 * Replace 'Insert_Your_API_Key_Here' with your API key and keep it within the double quotes.
 * ie. const std::string API_KEY = "667myExampleApiKey99099IAmAPoorBoyToo";
 * You can also change the number of trading days requested by changing 'START_DATE'.
 * It takes the format of YYYY-MM-DD
 * See World Trading Data API Documentation link above for more details.
 */


/*                  --API KEY--                    */
/*  (See instructions/examples in comments above)  */
/* ------------------------------------------------*/
const std::string API_KEY = "Insert_Your_API_Key_Here";         
const std::string START_DATE = "2019-06-08";              
/* ------------------------------------------------*/


enum class API
{
    HistoricalData,
    SymbolList
};

std::string CreateUrlForRequest(API service_request, const std::string& stock_symbol = "");       
void CheckResponse(const std::string& response);
void CheckApiKeyChanged();
