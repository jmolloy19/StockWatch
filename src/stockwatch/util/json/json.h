#ifndef STOCKWATCH_UTIL_JSON_JSON_H_
#define STOCKWATCH_UTIL_JSON_JSON_H_

#include "glog/logging.h"
#include "rapidjson/document.h"

namespace stockwatch {
namespace util {
namespace json {

std::string ToString(const rapidjson::Value& value);

template <class T>
bool AllEqualSize(T&& array_a, T&& array_b) {
    DCHECK(array_a.IsArray());
    DCHECK(array_b.IsArray());

    return array_a.Size() == array_b.Size();
}

template <class T, class... Ts>
bool AllEqualSize(T&& array_a, T&& array_b, Ts&&... arrays) {
    DCHECK(array_a.IsArray());
    DCHECK(array_b.IsArray());

    return array_a.Size() == array_b.Size() and AllEqualSize(array_b, std::forward<Ts>(arrays)...);
}

template <class T>
bool HasAllMembers(const rapidjson::Document& document, T&& name) {
    return document.HasMember(name);
}

template <class T, class... Ts>
bool HasAllMembers(const rapidjson::Document& document, T&& name, Ts&&... names) {
    return HasAllMembers(document, name) and HasAllMembers(document, std::forward<Ts>(names)...);
}

}  // namespace json
}  // namespace util
}  // namespace stockwatch

#endif  // STOCKWATCH_UTIL_JSON_JSON_H_