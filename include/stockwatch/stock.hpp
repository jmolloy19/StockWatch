#pragma once
#include <vector>
#include <algorithm>
#include <stockwatch/world_trading_api.hpp>
#include <utility/utility.hpp>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
// --Windows--
const std::string kDataFilesPath = ".\\datafiles\\"; 
#else
// --Linux/MacOS--
const std::string kDataFilesPath = "./datafiles/";
#endif

class Stock
{
	public:
		Stock() = delete;
		Stock(const Stock& s) = delete;
		Stock& operator=(const Stock &) = delete;
        Stock(const std::string& symbol);
		Stock(Stock &&) = default;
		bool ExhibitsHighTightFlag();
		void GetHistoricalData(bool read_from_file = false, bool write_to_file = false);
		friend std::ostream& operator << (std::ostream& out, const Stock& stock);
	private:
		void ParseHistoricalData(const std::string& historical_data);
		std::vector<double> closes_;
		std::vector<int> volumes_;
        std::string stock_name_;
		int num_trading_days_;
		File datafile_;
};