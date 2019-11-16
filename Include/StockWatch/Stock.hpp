#pragma once
#include <StockWatch/HistoricalData.hpp>
#include <Utility/Utility.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

class Stock
{
	public:
		Stock() = delete;
		Stock(const Stock& s) = delete;
		Stock& operator=(const Stock &) = delete;
        Stock(const std::string& symbol);
		Stock(Stock &&) = default;
		void analyze(bool write = false, bool read = false);
		friend std::ostream& operator << (std::ostream& out, const Stock& stock);
	private:
		void inputData(bool write = false, bool read = false);
		bool exhibitsHTF();
		std::vector<double> closes_;
		std::vector<int> volumes_;
        std::string name_;
		int numDays_;
        bool gotValidData_;	
};
