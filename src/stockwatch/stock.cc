#include "stockwatch/stock.h"

#include "glog/logging.h"

#include "rapidjson/error/en.h"
#include "stockwatch/pattern_detector.h"
#include "stockwatch/util/io/io.h"

namespace stockwatch {

Stock::Stock(const rapidjson::Value& security)
    : symbol_(security["symbol"].GetString()),
      description_(security["description"].GetString()),
      mic_code_(security["mic"].GetString()),
      security_type_(security["type"].GetString()) {}

void Stock::ParseFromJson(const std::string& json) {
    candles_.Parse(json.c_str());

    if (candles_.HasParseError()) {
        LOG(WARNING) << "JSON parse error (" << symbol_
                     << "): " << rapidjson::GetParseError_En(candles_.GetParseError());
        return;
    }

    if (not candles_.HasMember("c") or not candles_.HasMember("v") or not candles_.HasMember("s")) {
        VLOG(1) << "Invalid candles JSON (" << symbol_ << "): " << util::io::JsonValueToString(candles_);
        return;
    }

    if (std::strcmp(candles_["s"].GetString(), "ok") != 0) {
        VLOG(1) << "Non \"ok\" candles status (" << symbol_ << "): " << candles_["s"].GetString();
        return;
    }

    has_valid_candles_ = true;
}

bool Stock::ExhibitsHighTightFlag() const {
    return has_valid_candles_ ? PatternDetector::ExhibitsHighTightFlag(candles_) : false;
}

void Stock::Clear() {
    candles_.SetNull();
    candles_.GetAllocator().Clear();
}

const std::string& Stock::Symbol() const { return symbol_; }

const std::string& Stock::Description() const { return description_; }

const std::string& Stock::MicCode() const { return mic_code_; }

const std::string& Stock::SecurityType() const { return security_type_; }

}  // namespace stockwatch