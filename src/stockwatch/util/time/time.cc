#include "time.h"

#include <ctime>

#include "glog/logging.h"

namespace stockwatch {
namespace util {
namespace time {

std::chrono::system_clock::time_point NumDaysAgo(int days) {
    return std::chrono::system_clock::now() - std::chrono::hours(days * 24);
}

std::chrono::system_clock::time_point DaysFromNow(int days) {
    return std::chrono::system_clock::now() + std::chrono::hours(days * 24);
}

std::chrono::system_clock::time_point ToTimePoint(int64_t unix_time) {
    return std::chrono::system_clock::time_point(std::chrono::seconds(unix_time));
}

int NumDaysBetween(int64_t unix_time_start, int64_t unix_time_end) {
    DCHECK_GE(unix_time_end, unix_time_start);
    int64_t seconds_in_day = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::hours(24)).count();
    return (unix_time_end - unix_time_start) / seconds_in_day;
}

int64_t ToUnixTime(const std::string& unix_time) {
    return std::strtol(unix_time.c_str(), nullptr, 10);
}

int64_t ToUnixTime(const std::chrono::system_clock::time_point& time_point) {
    return std::chrono::duration_cast<std::chrono::seconds>(time_point.time_since_epoch()).count();
}

std::string CurrentLocalTime() {
    std::time_t timer;
    std::time(&timer);

    return std::ctime(&timer);
}

}  // namespace time
}  // namespace util
}  // namespace stockwatch
