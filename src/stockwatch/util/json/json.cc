#include "stockwatch/util/json/json.h"

#include "rapidjson/writer.h"

namespace stockwatch {
namespace util {
namespace json {

rapidjson::Value::ConstValueIterator MinElement(const rapidjson::Value::ConstValueIterator& first,
                                                const rapidjson::Value::ConstValueIterator& last) {
    if (first == last) {
        return first;
    }

    double min = 0;
    auto min_itr = first;

    for (auto itr = first; itr != last; ++itr) {
        DCHECK(itr->IsNumber());

        if (itr->IsInt()) {
            if (itr->GetInt() < min) {
                min = itr->GetInt();
                min_itr = itr;
            }
        } else if (itr->IsFloat()) {
            if (itr->GetFloat() < min) {
                min = itr->GetFloat();
                min_itr = itr;
            }
        } else if (itr->IsInt64()) {
            if (itr->GetInt64() < min) {
                min = itr->GetInt64();
                min_itr = itr;
            }
        } else if (itr->IsDouble()) {
            if (itr->GetDouble() < min) {
                min = itr->GetDouble();
                min_itr = itr;
            }
        } else {
            LOG(FATAL) << "Unexpected JSON Value type: " << itr->GetType();
        }
    }

    return min_itr;
}

rapidjson::Value::ConstValueIterator MaxElement(const rapidjson::Value::ConstValueIterator& first,
                                                const rapidjson::Value::ConstValueIterator& last) {
    if (first == last) {
        return first;
    }

    double max = 0;
    auto max_itr = first;

    for (auto itr = first; itr != last; ++itr) {
        DCHECK(itr->IsNumber());

        if (itr->IsInt()) {
            if (itr->GetInt() > max) {
                max = itr->GetInt();
                max_itr = itr;
            }
        } else if (itr->IsFloat()) {
            if (itr->GetFloat() > max) {
                max = itr->GetFloat();
                max_itr = itr;
            }
        } else if (itr->IsInt64()) {
            if (itr->GetInt64() > max) {
                max = itr->GetInt64();
                max_itr = itr;
            }
        } else if (itr->IsDouble()) {
            if (itr->GetDouble() > max) {
                max = itr->GetDouble();
                max_itr = itr;
            }
        } else {
            LOG(FATAL) << "Unexpected JSON Value type: " << itr->GetType();
        }
    }

    return max_itr;
}

std::string ToString(const rapidjson::Value& value) {
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);

    value.Accept(writer);

    return buffer.GetString();
}

}  // namespace json
}  // namespace util
}  // namespace stockwatch