#ifndef STOCKWATCH_UTIL_IO_IO_H_
#define STOCKWATCH_UTIL_IO_IO_H_

#include <experimental/filesystem>
#include <string>

#include "rapidjson/document.h"

namespace stockwatch {
namespace util {
namespace io {

bool ReadFromFile(const std::experimental::filesystem::path& path, std::string* contents);
bool WriteToFile(const std::experimental::filesystem::path& path, const std::string& contents);
bool ReadFromFile(const std::experimental::filesystem::path& path, rapidjson::Document* document);
bool WriteToFile(const std::experimental::filesystem::path& path, const rapidjson::Document& document);

}  // namespace io
}  // namespace util
}  // namespace stockwatch

#endif  // STOCKWATCH_UTIL_IO_IO_H_
