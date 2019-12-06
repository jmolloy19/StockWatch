#pragma once
#include <thread>
#include <stockwatch/stock.hpp>

const int MAX_THREADS = 10;
void AnalyzeStocks(const std::vector<std::string>& stock_symbols, const Options& options);