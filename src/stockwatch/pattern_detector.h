#ifndef STOCKWATCH_PATTERN_DETECTOR_H_
#define STOCKWATCH_PATTERN_DETECTOR_H_

#include <cmath>
#include <type_traits>

#include "glog/logging.h"

#include "rapidjson/document.h"

namespace stockwatch {

class PatternDetector {
   public:
    PatternDetector() = default;
    ~PatternDetector() = default;

    static bool ExhibitsHighTightFlag(const rapidjson::Value& candles);

   protected:
    static bool HasAtleastNumTradingDays(const rapidjson::Value& candles, size_t num_trading_days);
    static bool HasAtleastAverageClose(const rapidjson::Value& closes, double average_close);
    static bool HasAtleastAverageVolume(const rapidjson::Value& volumes, int average_volume);
    static bool HasZeroClose(const rapidjson::Value& closes);
    static bool HasZeroVolume(const rapidjson::Value& volumes);

    static rapidjson::Value::ConstValueIterator MinElement(const rapidjson::Value& array);
    static rapidjson::Value::ConstValueIterator MaxElement(const rapidjson::Value& array);

    static double Average(const rapidjson::Value& array);

    template <class T>
    static bool HasElementEqualTo(const rapidjson::Value& array, T value) {
        DCHECK(array.IsArray());
        DCHECK(std::is_arithmetic_v<T>);

        for (const auto& element : array.GetArray()) {
            DCHECK(element.IsNumber());

            if (element.IsDouble()) {
                if (std::fabs(element.GetDouble() - static_cast<double>(value)) < kComparisonEpsilon) {
                    return true;
                }
            } else {
                if (element.GetInt() == static_cast<int>(value)) {
                    return true;
                }
            }
        }
        return false;
    }

    template <class T>
    static bool HasElementLessThan(const rapidjson::Value& array, T value) {
        DCHECK(array.IsArray());
        DCHECK(std::is_arithmetic_v<T>);

        for (const auto& element : array.GetArray()) {
            DCHECK(element.IsNumber());

            if (element.IsDouble()) {
                if (element.GetDouble() < static_cast<double>(value)) {
                    return true;
                }
            } else {
                if (element.GetInt() < static_cast<int>(value)) {
                    return true;
                }
            }
        }

        return false;
    }

    template <class T>
    static bool HasElementGreaterThan(const rapidjson::Value& array, T value) {
        DCHECK(array.IsArray());
        DCHECK(std::is_arithmetic_v<T>);

        for (const auto& element : array.GetArray()) {
            DCHECK(element.IsNumber());

            if (element.IsDouble()) {
                if (element.GetDouble() > static_cast<double>(value)) {
                    return true;
                }
            } else {
                if (element.GetInt() > static_cast<int>(value)) {
                    return true;
                }
            }
        }

        return false;
    }

   private:
    static constexpr double kComparisonEpsilon = .01;
    static constexpr int kMinAverageVolume = 100000;
    static constexpr float kMinAverageClose = 1.0;
};

}  // namespace stockwatch
#endif  // STOCKWATCH_PATTERN_DETECTOR_H_