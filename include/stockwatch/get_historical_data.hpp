#pragma once
#include <iostream>
#include <curl/curl.h>
#include <stockwatch/api.hpp>
#include <utility/utility.hpp>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
// --Windows--
const std::string kDataFilesPath = ".\\datafiles\\"; 
const std::string kBadTickersPath = ".\\badtickers\\"; 
#else
// --Linux/MacOS--
const std::string kDataFilesPath = "./datafiles/";
const std::string kBadTickersPath = "./badtickers/"; 
#endif

void GetHistoricalData(const std::string& stock_symbol, std::string* historical_data, const Options& options);
static size_t RequestHistoricalDataCallback(void *contents, size_t size, size_t nmemb, void *userp);
void RequestHistoricalData(const std::string& symbol, std::string* read_buffer);

