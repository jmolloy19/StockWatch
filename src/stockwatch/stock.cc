#include "stockwatch/stock.h"

#include "glog/logging.h"

#include "stockwatch/pattern_detector.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"

namespace stockwatch {

void Stock::ParseFromJson(const std::string& json) {
    candles_.Parse(json.c_str());

    if (candles_.HasParseError()) {
        LOG(ERROR) << "JSON parse error (" << symbol_ << "): " << candles_.GetParseError();
        return;
    }

    if (not candles_.HasMember("c") or not candles_.HasMember("v") or not candles_.HasMember("s")) {
        LOG(WARNING) << "Invalid candles JSON (" << symbol_ << "): " << JsonDocToString(candles_);
        return;
    }

    if (std::strcmp(candles_["s"].GetString(), "ok") != 0) {
        VLOG(2) << "Non \"ok\" candles status (" << symbol_ << "): " << candles_["s"].GetString();
        return;
    }

    is_valid_ = true;
}

bool Stock::ExhibitsHighTightFlag() const { 
    return is_valid_ ? PatternDetector::ExhibitsHighTightFlag(candles_) : false; 
}

std::string Stock::ToString() const { return symbol_ + ":\n" + JsonDocToString(candles_); }

const std::string& Stock::Symbol() const { return symbol_; }

std::string Stock::JsonDocToString(const rapidjson::Document& document) {
    rapidjson::StringBuffer buffer;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);

    document.Accept(writer);

    return buffer.GetString();
}

}  // namespace stockwatch