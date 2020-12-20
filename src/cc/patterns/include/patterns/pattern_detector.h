#ifndef STOCKWATCH_PATTERNS_PATTERN_DETECTOR_H_
#define STOCKWATCH_PATTERNS_PATTERN_DETECTOR_H_

#include <type_traits>

#include "finnhub_resources.pb.h"

namespace sw {

class PatternDetector {
   public:
    PatternDetector() = default;
    ~PatternDetector() = default;

   protected:
     static bool HasAtleastNumTradingDays(int num_trading_days, const Candles& candles);
     static bool HasAtleastAverageVolume(int average_volume, const Candles& candles);
     static bool HasAtleastAverageClose(double average_close, const Candles& candles);
     static bool HasZeroClose(const Candles& candles);
     static bool HasZeroVolume(const Candles& candles);

    template <class Container>
    static double GetAverage(const Container& container) {
        double total = 0;
        int count = 0;
        for (const auto& element : container) {
            total += element;
            count++;
        }
        return total / count;
    }

    template <class Container, class Element>
    static bool HasElementEqualTo(const Container& container, Element value) {
        for (const auto& element : container) {
            if (IsEqual(element, value)) {
                return true;
            }
        }
        return false;
    }

    template <class Container, class Element>
    static bool HasElementLessThan(const Container& container, Element value) {
        for (const auto& element : container) {
            if (element < value) {
                return true;
            }
        }
        return false;
    }

    template <class Container, class Element>
    static bool HasElementGreaterThan(const Container& container, Element value) {
        for (const auto element : container) {
            if (element > value) {
                return true;
            }
        }
        return false;
    }

    template <class T, class U>
    static constexpr bool IsEqual(T e1, U e2) {
        if constexpr (std::is_floating_point_v<T> or std::is_floating_point_v<U>) {
            return std::abs(e1 - e2) < kComparisonEpsilon;
        } else {
            return e1 == e2;
        }
    }

    private:
    static constexpr double kComparisonEpsilon = .01;
};

}  // namespace sw
#endif  // STOCKWATCH_PATTERNS_PATTERN_DETECTOR_H_