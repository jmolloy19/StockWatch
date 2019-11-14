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
		Stock& operator=(const Stock &) = delete;
        Stock(const std::string& symbol);
		Stock(Stock &&) = default;
		void analyze();
		friend std::ostream& operator << (std::ostream& out, const Stock& stock);
	private:
		void inputData();
		bool exhibitsHTF();
		std::vector<double> closes_;
		std::vector<int> volumes_;
        std::string name_;
		int numDays_;
        bool gotValidData_;	
};
