#pragma once
#include <vector>
#include <string>
#include <iostream>

class Stock
{
	public:
		Stock() = delete;
        Stock(const std::string& symbol);
		int getNumDays() const;
		void checkForHTF();
		friend std::ostream& operator << (std::ostream& out, const Stock& stock);
	private:
		void inputData(const std::string& historicalData);
		std::vector<double> closes_;
		std::vector<int> volumes_;
        std::string name_;
		int numDays_;
        bool gotValidData_;	
};
