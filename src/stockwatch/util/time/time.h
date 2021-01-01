#ifndef STOCKWATCH_UTIL_TIME_TIME_H_
#define STOCKWATCH_UTIL_TIME_TIME_H_

#include <chrono>
#include <string>

namespace stockwatch {
namespace util {
namespace time {

std::chrono::system_clock::time_point Now();
std::chrono::system_clock::time_point NumDaysAgo(int days);
std::chrono::system_clock::time_point ToTimePoint(int64_t unix_time);

int64_t ToUnixTime(const std::string& unix_time);
int64_t ToUnixTime(const std::chrono::system_clock::time_point& time_point);




}  // namespace time
}  // namespace util
}  // namespace stockwatch

#endif  // STOCKWATCH_UTIL_TIME_TIME_H_
