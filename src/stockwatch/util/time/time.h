#ifndef STOCKWATCH_UTIL_TIME_TIME_H_
#define STOCKWATCH_UTIL_TIME_TIME_H_

#include <chrono>
#include <string>

namespace stockwatch {
namespace util {
namespace time {

std::chrono::system_clock::time_point NumDaysAgo(int days);
std::chrono::system_clock::time_point DaysFromNow(int days);
std::chrono::system_clock::time_point ToTimePoint(int64_t unix_time);

int NumDaysBetween(int64_t unix_time_start, int64_t unix_time_end);

int64_t ToUnixTime(const std::string& unix_time);
int64_t ToUnixTime(const std::chrono::system_clock::time_point& time_point);

template <class Rep, class Period>
int64_t ToUnixTime(const std::chrono::duration<Rep, Period>& duration) {
    static_assert(std::is_same_v<Rep, int64_t>);
    return std::chrono::duration_cast<std::chrono::seconds>(duration).count();
}

std::string CurrentLocalTime();

}  // namespace time
}  // namespace util
}  // namespace stockwatch

#endif  // STOCKWATCH_UTIL_TIME_TIME_H_
