#ifndef STOCKWATCH_PATTERNS_HIGH_TIGHT_FLAG_H_
#define STOCKWATCH_PATTERNS_HIGH_TIGHT_FLAG_H_

#include "stockwatch/candle.h"

namespace stockwatch {
namespace patterns {

class HighTightFlag {
   public:
    HighTightFlag() = default;
    ~HighTightFlag() = default;

    static bool ExhibitsPattern(const std::vector<Candle>& candles);
    static std::vector<Candle>::const_iterator FlagPoleBottom(const std::vector<Candle>& candles);
    static std::vector<Candle>::const_iterator FlagPoleTop(const std::vector<Candle>& candles);
};

}  // namespace patterns
}  // namespace stockwatch
#endif  // STOCKWATCH_PATTERNS_HIGH_TIGHT_FLAG_H_