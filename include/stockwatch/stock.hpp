#pragma once
#include <vector>
#include <algorithm>
#include <utility/files.hpp>
#include <stockwatch/world_trading_data.hpp>

class Stock
{
	public:
		Stock() = delete;
		Stock(const Stock& s) = delete;
		Stock& operator=(const Stock &) = delete;
        Stock(const std::string& stock_symbol, bool read_file = false, bool write_file = false);
		Stock(Stock &&) = default;
		bool ExhibitsHighTightFlag();
		friend std::ostream& operator << (std::ostream& out, const Stock& stock);
	private:
		void Init();
		void GetHistoricalData(std::string* data_buffer);
		void ParseHistoricalData(const std::string& data_buffer);
		bool read_file_;
		bool write_file_;
		std::vector<double> closes_;
		std::vector<int> volumes_;
        std::string symbol_;
		int num_trading_days_ = 0;
};