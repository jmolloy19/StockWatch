#include "stockwatch/stock.h"

#include "glog/logging.h"

#include "stockwatch/pattern_detector.h"
#include "stockwatch/util/rapidjson/prettywriter.h"
#include "stockwatch/util/rapidjson/stringbuffer.h"

namespace sw {

bool Stock::ParseFromJson(const std::string& json) {
    candles_.Parse(json.c_str());

    if (candles_.HasParseError()) {
        LOG(ERROR) << "JSON parse error (" << symbol_ << "): " << candles_.GetParseError();
        return false;
    }

    return true;
}

bool Stock::ExhibitsHighTightFlag() const { return PatternDetector::ExhibitsHighTightFlag(candles_); }

std::string Stock::ToString() const {
    rapidjson::StringBuffer buffer;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);

    candles_.Accept(writer);

    return symbol_ + ":\n" + buffer.GetString();
}

const std::string& Stock::Symbol() const { return symbol_; }

}  // namespace sw