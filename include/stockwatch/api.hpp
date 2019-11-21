#pragma once
#include <string>

/**
 * World Trading Data API
 * Documention: https://www.worldtradingdata.com/documentation#full-history
 * Test URI : https://api.worldtradingdata.com/api/v1/history?symbol=AAPL&sort=newest&api_token=5mTLsc1GwvEkMo3AGEu00wh5BVgzz2Jwdg14lm66ohX9cnGFntMpZyJdM6WA&output=csv&date_from=2019-06-08
 */
const std::string url_base = "https://api.worldtradingdata.com/api/v1/history?symbol=";
const std::string sort = "&sort=newest";
const std::string api_key = "&api_token=5mTLsc1GwvEkMo3AGEu00wh5BVgzz2Jwdg14lm66ohX9cnGFntMpZyJdM6WA";
const std::string output = "&output=csv";
const std::string start_date = "&date_from=2019-06-08";