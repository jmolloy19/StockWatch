#pragma once
#include <string>

/**
 * For World Trading API
 * Documention: https://www.worldtradingdata.com/documentation#full-history
 * Test URI : https://api.worldtradingdata.com/api/v1/history?symbol=AAPL&sort=newest&api_token=5mTLsc1GwvEkMo3AGEu00wh5BVgzz2Jwdg14lm66ohX9cnGFntMpZyJdM6WA&output=csv&date_from=2019-06-08
 */
const std::string urlBase = "https://api.worldtradingdata.com/api/v1/history?symbol=";
const std::string sort = "&sort=newest";
const std::string apiKey = "&api_token=5mTLsc1GwvEkMo3AGEu00wh5BVgzz2Jwdg14lm66ohX9cnGFntMpZyJdM6WA";
const std::string output = "&output=csv";
const std::string startDate = "&date_from=2019-06-08";