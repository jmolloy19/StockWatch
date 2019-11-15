#pragma once
#include <string>
#include <vector>
#include <curl/curl.h>
#include <iostream>
#include <algorithm>

static size_t writeSymbolsCallback(void *contents, size_t size, size_t nmemb, void *userp);
bool fetchStockSymbols(std::string* readBuffer, bool nyse = false);
void createSymbolList(std::vector<std::string>* symbols, bool nyse);
bool isValidSymbol(const std::string& symbol);
