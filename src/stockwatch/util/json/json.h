#ifndef STOCKWATCH_UTIL_JSON_JSON_H_
#define STOCKWATCH_UTIL_JSON_JSON_H_

#include "glog/logging.h"
#include "rapidjson/document.h"

namespace stockwatch {
namespace util {
namespace json {

rapidjson::Value::ConstValueIterator MinElement(const rapidjson::Value::ConstValueIterator& first,
                                                const rapidjson::Value::ConstValueIterator& last);

rapidjson::Value::ConstValueIterator MaxElement(const rapidjson::Value::ConstValueIterator& first,
                                                const rapidjson::Value::ConstValueIterator& last);

std::string ToString(const rapidjson::Value& value);

template <class T>
bool AllEqualSize(T&& array_1, T&& array_2) {
    DCHECK(array_1.IsArray());
    DCHECK(array_2.IsArray());

    return array_1.Size() == array_2.Size();
}

template <class T, class... Ts>
bool AllEqualSize(T&& array_1, T&& array_2, Ts&&... arrays) {
    DCHECK(array_1.IsArray());
    DCHECK(array_2.IsArray());

    return array_1.Size() == array_2.Size() and AllEqualSize(array_2, std::forward<Ts>(arrays)...);
}

template <class T>
bool HasAllMembers(const rapidjson::Document& document, T&& name) {
    return document.HasMember(name);
}

template <class T, class... Ts>
bool HasAllMembers(const rapidjson::Document& document, T&& name, Ts&&... names) {
    return HasAllMembers(document, name) and HasAllMembers(document, std::forward<Ts>(names)...);
}

template <class T>
T Average(const rapidjson::Value::ConstValueIterator& first, const rapidjson::Value::ConstValueIterator& last) {
    std::ptrdiff_t size = last - first;

    if (size == 0) {
        return 0;
    }

    double total = 0;

    for (auto itr = first; itr != last; itr++) {
        if constexpr (std::is_same_v<T, int>) {
            total += itr->GetInt();
        } else if constexpr (std::is_same_v<T, float>) {
            total += itr->GetFloat();
        } else if constexpr (std::is_same_v<T, int64_t>) {
            total += itr->GetInt64();
        } else if constexpr (std::is_same_v<T, double>) {
            total += itr->GetDouble();
        } else {
            LOG(FATAL) << "Invalid type: " << typeid(T).name();
        }
    }

    return static_cast<T>(total / size);
}

}  // namespace json
}  // namespace util
}  // namespace stockwatch

#endif  // STOCKWATCH_UTIL_JSON_JSON_H_