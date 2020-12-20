#include "patterns/pattern_detector.h"

namespace sw {

bool PatternDetector::HasAtleastNumTradingDays(int num_trading_days, const Candles& candles) {
    return candles.c().size() >= num_trading_days;
}

bool PatternDetector::HasAtleastAverageVolume(int average_volume, const Candles& candles) {
    return GetAverage(candles.v()) >= average_volume;
}

bool PatternDetector::HasAtleastAverageClose(double average_close, const Candles& candles) {
    return GetAverage(candles.c()) >= average_close;
}

bool PatternDetector::HasZeroClose(const Candles& candles) {
    return HasElementEqualTo(candles.c(), 0);
}

bool PatternDetector::HasZeroVolume(const Candles& candles) {
    return HasElementEqualTo(candles.v(), 0);
}

}  // namespace sw