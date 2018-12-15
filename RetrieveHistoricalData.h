#ifndef RETRIEVE_HISTORICAL_DATA_H
#define RETRIEVE_HISTORICAL_DATA_H
#include<iostream>
#include<string>
using namespace std;

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp);
string fetchHistoricalData(string symbol);

#endif
