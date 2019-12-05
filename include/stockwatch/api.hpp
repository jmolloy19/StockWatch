#pragma once
#include <string>

/**
 * World Trading Data API
 * Documention: https://www.worldtradingdata.com/documentation#full-history
 * Replace 'Insert_Your_API_Key_Here' with your API key and keep it within the double quotes.
 * ie. const std::string API_KEY = "myExampleApiKey_IAmAPoorBoyToo_5318008";
 * You can also change the number of trading days requested by changing 'START_DATE'.
 * It takes the format of YYYY-MM-DD
 * See World Trading Data API Documentation link above for more details.
 */

const std::string API_KEY = "Insert_Your_API_Key_Here";
const std::string START_DATE = "2019-06-08";

