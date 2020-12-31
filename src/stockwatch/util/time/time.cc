#include "time.h"

namespace stockwatch {
namespace util {
namespace time {

std::chrono::system_clock::time_point Now() { return std::chrono::system_clock::now(); }

std::chrono::system_clock::time_point NumDaysAgo(int days) {
    return std::chrono::system_clock::now() - std::chrono::hours(days * 24);
}

std::string ToString(const std::chrono::system_clock::time_point& time_point) {
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(time_point.time_since_epoch()).count();
    return std::to_string(seconds);
}

std::chrono::system_clock::time_point FromString(const std::string& unix_time) {
    int64_t seconds = std::strtol(unix_time.c_str(), nullptr, 10);
    return std::chrono::system_clock::time_point(std::chrono::seconds(seconds));
}

}  // namespace time
}  // namespace util
}  // namespace stockwatch
