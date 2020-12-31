#ifndef STOCKWATCH_HIGH_TIGHT_FLAG_H_
#define STOCKWATCH_HIGH_TIGHT_FLAG_H_

#include "stockwatch/candle.h"

namespace stockwatch {
namespace pattern {

class HighTightFlag {
   public:
    HighTightFlag() = default;
    ~HighTightFlag() = default;

    static bool ExhibitsPattern(const std::vector<Candle>& candles);
    static std::vector<Candle>::const_iterator FlagPoleBottom(const std::vector<Candle>& candles);
    static std::vector<Candle>::const_iterator FlagPoleTop(const std::vector<Candle>& candles);
};

}  // namespace pattern
}  // namespace stockwatch
#endif  // STOCKWATCH_HIGH_TIGHT_FLAG_H_