#include "stockwatch/pattern_detector.h"

#include <algorithm>

#include "stockwatch/util/rapidjson/prettywriter.h"
#include "stockwatch/util/rapidjson/stringbuffer.h"

namespace sw {

bool PatternDetector::ExhibitsHighTightFlag(const rapidjson::Document& candles) {
    // if (not candles.HasMember("candles")) {
    //     return false;
    // }

    // if (not candles["candles"].HasMember("c") or not candles["candles"].HasMember("v") or not
    // candles["candles"].HasMember("s")) {
    //     return false;
    // }

    // if (std::strcmp(candles["candles"]["s"].GetString(), "ok") != 0) {
    //     return false;
    // }

    // const auto& closes = candles["candles"]["c"];
    // const auto& volumes = candles["candles"]["v"];
    IsValidCandles()
    const auto& closes = candles["c"];
    const auto& volumes = candles["v"];

    const auto* flag_pole_bottom = MinElement(closes);
    const auto* flag_pole_top = MaxElement(closes);

    // Price must double within 45 trading day period.
    if (HasZeroClose(closes) or flag_pole_top->GetFloat() / flag_pole_bottom->GetFloat() < 2.0) {
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

    if (HasAtleastAverageClose(kMinAverageClose, closes)) {
        return false;
    }

    if (HasZeroVolume(candles) or not HasAtleastAverageVolume(kMinAverageVolume, volumes)) {
        return false;
    }

    return true;
}

bool PatternDetector::IsValidJsonDoc(const rapidjson::Document& candles) {
    if (candles.HasParseError()) {
        return false;
    }

    if (not candles.HasMember("c") or not candles.HasMember("v") or not candles.HasMember("s")) {
        VLOG(1) << "Invalid candles JSON document: " << JsonDocToString(candles);
        return false;
    }

    if (std::strcmp(candles["s"].GetString(), "ok") != 0) {
        return false;
    }

    if (not HasAtleastNumTradingDays(45, candles)) {
        return false;
    }

    return true;
}

bool PatternDetector::HasAtleastNumTradingDays(size_t num_trading_days, const rapidjson::Document& candles) {
    // return candles["candles"]["c"].GetArray().Size() >= num_trading_days and
    // candles["candles"]["v"].GetArray().Size() >= num_trading_days;
    return candles["c"].GetArray().Size() >= num_trading_days and candles["v"].GetArray().Size() >= num_trading_days;
}

bool PatternDetector::HasAtleastAverageClose(double average_close, const rapidjson::Value& closes) {
    return Average(closes) >= average_close;
}

bool PatternDetector::HasAtleastAverageVolume(int average_volume, const rapidjson::Value& volumes) {
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

    if (array.Size() <= 1) {
        return array.Begin();
    }

    double min = 0;
    auto min_itr = array.GetArray().Begin();

    for (auto itr = array.GetArray().Begin(); itr != array.GetArray().End(); ++itr) {
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

    if (array.GetArray().Size() <= 1) {
        return array.GetArray().Begin();
    }

    double max = 0;
    auto max_itr = array.GetArray().Begin();

    for (auto itr = array.GetArray().Begin(); itr != array.GetArray().End(); ++itr) {
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

std::string PatternDetector::JsonDocToString(const rapidjson::Document& document) {
    rapidjson::StringBuffer buffer;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);

    document.Accept(writer);

    return buffer.GetString();
}

}  // namespace sw