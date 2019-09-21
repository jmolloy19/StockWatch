#pragma once
#include <string>

static size_t writeDataCallback(void *contents, size_t size, size_t nmemb, void *userp);
void fetchHistoricalData(const std::string& symbol, std::string* readBuffer);

