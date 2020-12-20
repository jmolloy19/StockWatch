#include "patterns/high_tight_flag.h"

#include <algorithm>

namespace sw {

bool HighTightFlag::ExhibitsPattern(const Candles& candles) {
    if (not HasAtleastNumTradingDays(60, candles)) {
        return false;
    }

    if (HasZeroClose(candles) or not HasAtleastAverageClose(kMinAverageClose, candles)) {
        return false;
    }

    if (HasZeroVolume(candles) or not HasAtleastAverageVolume(kMinAverageVolume, candles)) {
        return false;
    }

    auto begin = candles.c().cbegin();
    auto end = candles.c().cend();
    auto flag_pole_bottom = std::min_element(begin, end);
    auto flag_pole_top = std::max_element(begin, end);

    // Price must atleast double from lowest low to highest high.
    if (*flag_pole_top / *flag_pole_bottom < 1.9) {
        return false;
    }

    // Flag pole top must be within 15 tradying days of most recent trading day
    std::ptrdiff_t flag_length = std::distance(flag_pole_top, end);
    if (flag_length > 15) {
        return false;
    }

    // Closes during flag period must not drop below 80% of the flag pole height.
    float flag_pole_height = *flag_pole_top - *flag_pole_bottom;

    for (auto itr = flag_pole_top; itr != end; itr++) {
        if (*itr - *flag_pole_bottom < 0.8 * flag_pole_height) {
            return false;
        }
    }

    return true;
}

google::protobuf::RepeatedField<float>::const_iterator HighTightFlag::FlagPoleBottom(const Candles& candles) {
    return std::min_element(candles.c().cbegin(), candles.c().cend());
}

google::protobuf::RepeatedField<float>::const_iterator HighTightFlag::FlagPoleTop(const Candles& candles) {
    return std::max_element(candles.c().cbegin(), candles.c().cend());
}

}  // namespace sw