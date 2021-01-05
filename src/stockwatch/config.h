#ifndef STOCKWATCH_CONFIG_H_
#define STOCKWATCH_CONFIG_H_

#include <experimental/filesystem>

#include "cxxopts/cxxopts.h"

namespace stockwatch {

class Config {
   public:
    Config() = delete;
    Config(const cxxopts::ParseResult& options);
    ~Config() = default;

    void LogConfig() const;
    const std::experimental::filesystem::path& JsonsDir() const;
    const std::string& FinnhubApiKey() const;
    const std::string& EmailPassword() const;
    std::string DbConnectionString() const;
    bool IsWriteToFile() const;
    bool IsReadFromFile() const;

   private:
    const cxxopts::ParseResult options_;
};

}  // namespace stockwatch
#endif  // STOCKWATCH_CONFIG_H_
