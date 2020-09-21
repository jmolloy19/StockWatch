#ifndef STOCKWATCH_STOCK_H_
#define STOCKWATCH_STOCK_H_

#include "candle.pb.h"

namespace sw {

class Stock {
	public:
		Stock() = default;
		bool ExhibitsHighTightFlag() const;
		void Init(const std::string& ticker);
		void Analyze();
		std::string ToString() const;
		std::string Ticker() const;
	private:
		Candle candle_;
        std::string ticker_;
		bool exhibits_high_tight_flag_{false};
};

}  // namespace sw
#endif  // STOCKWATCH_STOCK_H_