#pragma once
#include <string>
#include <curl/curl.h>
#include <iostream>
#include <StockWatch/Api.hpp>

static size_t writeDataCallback(void *contents, size_t size, size_t nmemb, void *userp);
void fetchHistoricalData(const std::string& symbol, std::string* readBuffer);

