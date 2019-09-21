#pragma once
#include <string>
#include <vector>

static size_t writeSymbolsCallback(void *contents, size_t size, size_t nmemb, void *userp);
void fetchStockSymbols(std::string* readBuffer);
void createSymbolList(std::vector<std::string>* symbols);
bool checkValidSymbol(const std::string& symbol);
