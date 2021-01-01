#include "stockwatch/patterns/high_tight_flag.h"

#include "glog/logging.h"

namespace stockwatch {
namespace patterns {

bool HighTightFlag::ExhibitsPattern(const std::vector<Candle>& candles) {
    auto flag_pole_bottom = FlagPoleBottom(candles);
    auto flag_pole_top = FlagPoleTop(candles);

    // Flag pole bottom must occur before flag pole top.
    if (std::distance(flag_pole_bottom, flag_pole_top) < 0) {
        return false;
    }

    // Price must atleast doible from lowest low to highest high.
    if (flag_pole_bottom->low * 2.0 > flag_pole_top->high) {
        return false;
    }

    // Price double must occur within 2 months (~45 trading days).
    if (std::distance(flag_pole_bottom, flag_pole_top) > 45) {
        return false;
    }

    std::ptrdiff_t flag_length = std::distance(flag_pole_top, candles.cend());

    // Length of flag must be atleast 5 days and no more than 15 days.
    if (flag_length < 5 or flag_length > 15) {
        return false;
    }

    float flag_pole_height = flag_pole_top->high - flag_pole_bottom->low;

    // Closes during flag period must not drop below 80% of flag pole height.
    for (auto itr = flag_pole_top; itr != candles.cend(); itr++) {
        if (itr->close - flag_pole_bottom->low < 0.80 * flag_pole_height) {
            return false;
        }
    }

    return true;
}

std::vector<Candle>::const_iterator HighTightFlag::FlagPoleBottom(const std::vector<Candle>& candles) {
    DCHECK(candles.size() >= 60);
    return MinLow(candles.cend() - 60, candles.cend());
}

std::vector<Candle>::const_iterator HighTightFlag::FlagPoleTop(const std::vector<Candle>& candles) { 
    DCHECK(candles.size() >= 60);
    return MaxHigh(candles.cend() - 60, candles.cend()); 
}

}  // namespace patterns
}  // namespace stockwatch
