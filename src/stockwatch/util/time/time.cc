#include "time.h"

namespace stockwatch {
namespace util {
namespace time {

std::chrono::system_clock::time_point Now() { return std::chrono::system_clock::now(); }

std::chrono::system_clock::time_point NumDaysAgo(int days) {
    return std::chrono::system_clock::now() - std::chrono::hours(days * 24);
}

std::chrono::system_clock::time_point ToTimePoint(int64_t unix_time) {
    return std::chrono::system_clock::time_point(std::chrono::seconds(unix_time));
}

int64_t ToUnixTime(const std::string& unix_time) {
    return std::strtol(unix_time.c_str(), nullptr, 10);
}

int64_t ToUnixTime(const std::chrono::system_clock::time_point& time_point) {
    return std::chrono::duration_cast<std::chrono::seconds>(time_point.time_since_epoch()).count();
}


}  // namespace time
}  // namespace util
}  // namespace stockwatch
