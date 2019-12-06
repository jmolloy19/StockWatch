#pragma once
#include <vector>
#include <algorithm>
#include <stockwatch/get_historical_data.hpp>

class Stock
{
	public:
		Stock() = delete;
		Stock(const Stock& s) = delete;
		Stock& operator=(const Stock &) = delete;
        Stock(const std::string& symbol);
		Stock(Stock &&) = default;
		void Analyze(const Options& options);
		friend std::ostream& operator << (std::ostream& out, const Stock& stock);
	private:
		void InputHistoricalData(const std::string& historical_data);
		bool ExhibitsHighTightFlag();
		std::vector<double> closes_;
		std::vector<int> volumes_;
        std::string stock_name_;
		int number_of_days_;
};