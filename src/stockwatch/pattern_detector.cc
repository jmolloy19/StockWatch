#include "stockwatch/pattern_detector.h"

#include <algorithm>

namespace stockwatch {

bool PatternDetector::ExhibitsHighTightFlag(const rapidjson::Value& candles) {
    DCHECK(not candles.IsNull());

    if (not HasAtleastNumTradingDays(candles, 45)) {
        return false;
    }

    const auto& closes = candles["c"];
    const auto& volumes = candles["v"];

    const auto* flag_pole_bottom = MinElement(closes);
    const auto* flag_pole_top = MaxElement(closes);

    // Price must double within 45 trading day period.
    if (flag_pole_bottom->GetFloat() == 0 or flag_pole_top->GetFloat() / flag_pole_bottom->GetFloat() < 2.0) {
        return false;
    }

    std::ptrdiff_t flag_length = std::distance(flag_pole_top, closes.End());

    // Length of flag must be atleast 5 days and no more than 15 days
    if (flag_length < 5 or flag_length > 15) {
        return false;
    }

    // Closes during flag period must not drop below 80% of the flag pole height.
    float flag_pole_height = flag_pole_top->GetFloat() - flag_pole_bottom->GetFloat();

    for (auto itr = flag_pole_top; itr != closes.End(); itr++) {
        if (itr->GetFloat() - flag_pole_bottom->GetFloat() < 0.8 * flag_pole_height) {
            return false;
        }
    }

    if (HasZeroClose(closes) or not HasAtleastAverageClose(closes, kMinAverageClose)) {
        return false;
    }

    if (HasZeroVolume(volumes) or not HasAtleastAverageVolume(volumes, kMinAverageVolume)) {
        return false;
    }

    return true;
}

bool PatternDetector::HasAtleastNumTradingDays(const rapidjson::Value& candles, size_t num_trading_days) {
    return candles["c"].Size() >= num_trading_days and candles["v"].Size() >= num_trading_days;
}

bool PatternDetector::HasAtleastAverageClose(const rapidjson::Value& closes, double average_close) {
    return Average(closes) >= average_close;
}

bool PatternDetector::HasAtleastAverageVolume(const rapidjson::Value& volumes, int average_volume) {
    return Average(volumes) >= average_volume;
}

bool PatternDetector::HasZeroClose(const rapidjson::Value& closes) { return HasElementEqualTo(closes, 0); }

bool PatternDetector::HasZeroVolume(const rapidjson::Value& volumes) { return HasElementEqualTo(volumes, 0); }

double PatternDetector::Average(const rapidjson::Value& array) {
    DCHECK(array.IsArray());

    double total = 0;

    for (const auto& element : array.GetArray()) {
        DCHECK(element.IsNumber());

        if (element.IsFloat()) {
            total += element.GetFloat();
        } else if (element.IsInt()) {
            total += element.GetInt();
        } else if (element.IsUint()) {
            total += element.GetUint();
        } else if (element.IsInt64()) {
            total += element.GetInt64();
        } else if (element.IsUint64()) {
            total += element.GetUint64();
        }
    }

    return array.Size() == 0 ? 0 : total / array.Size();
}

rapidjson::Value::ConstValueIterator PatternDetector::MinElement(const rapidjson::Value& array) {
    DCHECK(array.IsArray());

    const auto begin = array.Begin();
    const auto end = array.End();
    const size_t size = array.Size();

    if (size <= 1) {
        return begin;
    }

    double min = 0;
    auto min_itr = begin;

    for (auto itr = begin; itr != end; ++itr) {
        DCHECK(itr->IsNumber());

        if (itr->IsDouble()) {
            if (itr->GetDouble() < min) {
                min = itr->GetDouble();
                min_itr = itr;
            }
        } else if (itr->IsInt()) {
            if (itr->GetInt() < min) {
                min = itr->GetInt();
                min_itr = itr;
            }
        } else if (itr->IsUint()) {
            if (itr->GetUint() < min) {
                min = itr->GetInt();
                min_itr = itr;
            }
        } else if (itr->IsInt64()) {
            if (itr->GetInt64() < min) {
                min = itr->GetInt64();
                min_itr = itr;
            }
        } else if (itr->IsUint64()) {
            if (itr->GetUint64() < min) {
                min = itr->GetUint64();
                min_itr = itr;
            }
        }
    }

    return min_itr;
}

rapidjson::Value::ConstValueIterator PatternDetector::MaxElement(const rapidjson::Value& array) {
    DCHECK(array.IsArray());

    const auto begin = array.GetArray().Begin();
    const auto end = array.GetArray().End();
    const size_t size = array.GetArray().Size();

    if (size <= 1) {
        return begin;
    }

    double max = 0;
    auto max_itr = begin;

    for (auto itr = begin; itr != end; ++itr) {
        DCHECK(itr->IsNumber());

        if (itr->IsDouble()) {
            if (itr->GetDouble() > max) {
                max = itr->GetDouble();
                max_itr = itr;
            }
        } else if (itr->IsInt()) {
            if (itr->GetInt() > max) {
                max = itr->GetInt();
                max_itr = itr;
            }
        } else if (itr->IsUint()) {
            if (itr->GetUint() > max) {
                max = itr->GetInt();
                max_itr = itr;
            }
        } else if (itr->IsInt64()) {
            if (itr->GetInt64() > max) {
                max = itr->GetInt64();
                max_itr = itr;
            }
        } else if (itr->IsUint64()) {
            if (itr->GetUint64() > max) {
                max = itr->GetUint64();
                max_itr = itr;
            }
        }
    }

    return max_itr;
}

}  // namespace stockwatch