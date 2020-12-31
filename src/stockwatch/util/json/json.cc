#include "stockwatch/util/json/json.h"

#include "rapidjson/writer.h"

namespace stockwatch {
namespace util {
namespace json {


std::string ToString(const rapidjson::Value& value) {
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);

    value.Accept(writer);

    return buffer.GetString();
}

}  // namespace json
}  // namespace util
}  // namespace stockwatch