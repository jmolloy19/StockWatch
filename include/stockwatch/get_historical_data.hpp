#pragma once
#include <string>
#include <curl/curl.h>
#include <iostream>
#include <stockwatch/api.hpp>
#include <utility/utility.hpp>

void GetHistoricalData(const std::string& stock_symbol, std::string* historical_data, bool read_from_file = false);
static size_t RequestHistoricalDataCallback(void *contents, size_t size, size_t nmemb, void *userp);
void RequestHistoricalData(const std::string& symbol, std::string* read_buffer);

