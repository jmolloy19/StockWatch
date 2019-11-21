#pragma once
#include <string>
#include <vector>
#include <curl/curl.h>
#include <iostream>
#include <algorithm>

void GetStockSymbols(std::vector<std::string>* symbols, bool nyse);
bool IsValidStockSymbol(const std::string& symbol);
static size_t RequestStockSymbolsCallback(void *contents, size_t size, size_t nmemb, void *userp);
bool RequestStockSymbols(std::string* read_buffer, bool nyse = false);
