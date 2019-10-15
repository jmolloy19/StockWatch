#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

class Stock
{
	public:
		Stock() = delete;
		Stock(const Stock& s) = delete;
        Stock(const std::string& symbol);
		bool exhibitsHTF();
		friend std::ostream& operator << (std::ostream& out, const Stock& stock);
	private:
		void inputData(const std::string& historicalData);
		std::vector<double> closes_;
		std::vector<int> volumes_;
        std::string name_;
		int numDays_;
        bool gotValidData_;	
};
