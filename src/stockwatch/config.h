#ifndef STOCKWATCH_CONFIG_H_
#define STOCKWATCH_CONFIG_H_

#include <experimental/filesystem>

#include "cxxopts/cxxopts.h"

namespace stockwatch {

class Config {
   public:
    Config() = delete;
    Config(const cxxopts::ParseResult& args);
    ~Config() = default;

    void Log() const;

    const std::experimental::filesystem::path& JsonsDir() const;
    const std::string& FinnhubApiKey() const;
    std::string DbConnectionString() const;
    bool IsWriteToFile() const;
    bool IsReadFromFile() const;

   private:
    const std::string finnhub_api_key_;
    const std::experimental::filesystem::path jsons_dir_{STOCKWATCH_JSONS_DIR};

    const bool write_to_file_{false};
    const bool read_from_file_{false};

    const std::string db_name_;
    const std::string db_user_;
    const std::string db_password_;
    const std::string db_address_;
    const std::string db_port_;
};

}  // namespace stockwatch
#endif  // STOCKWATCH_CONFIG_H_
