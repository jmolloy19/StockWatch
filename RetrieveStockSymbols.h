#ifndef RETRIEVE_STOCK_SYMBOLS_H
#define RETRIEVE_STOCK_SYMBOLS_H
#include <string>
#include <vector>
using namespace std;

static size_t WriteCallback_Symbol(void *contents, size_t size, size_t nmemb, void *userp);
string fetchStockSymbols();
vector<string> createSymbolList(const string &readBuffer);
bool checkValidSymbol(string symb);

#endif

