#include "stockwatch/config.h"

#include "glog/logging.h"

namespace stockwatch {

Config::Config(const cxxopts::ParseResult& options) : options_(options) {}

void Config::LogConfig() const {
    LOG(INFO) << "\n----- StockWatch Configuration -----"
              << "\nDB Name:         " << options_["db_name"].as<std::string>()
              << "\nDB User:         " << options_["db_user"].as<std::string>()
              << "\nDB Address:      " << options_["db_address"].as<std::string>()
              << "\nDB Port:         " << options_["db_port"].as<std::string>()
              << "\nWrite To File:   "
              << (IsWriteToFile() ? "true\nWrite File Dir:  " + JsonsDir().string() : "false")
              << "\nRead From File:  "
              << (IsReadFromFile() ? "true\nRead File Dir:   " + JsonsDir().string() : "false")
              << "\nLog Verbosity:   " << FLAGS_v << "\n------------------------------------";
}

const std::experimental::filesystem::path& Config::JsonsDir() const {
    return options_["jsons_dir"].as<std::experimental::filesystem::path>();
}

const std::string& Config::FinnhubApiKey() const {
    return options_["finnhub_api_key"].as<std::string>();
}

const std::string& Config::EmailPassword() const {
    return options_["email_password"].as<std::string>();
}

std::string Config::DbConnectionString() const {
    std::string connection_string("dbname = " + options_["db_name"].as<std::string>());
    connection_string.append(" user = " + options_["db_user"].as<std::string>());
    connection_string.append(" password = " + options_["db_password"].as<std::string>());
    connection_string.append(" hostaddr = " + options_["db_address"].as<std::string>());
    connection_string.append(" port = " + options_["db_port"].as<std::string>());

    return connection_string;
}

bool Config::IsWriteToFile() const { return options_["write_to_file"].as<bool>(); }

bool Config::IsReadFromFile() const { return options_["read_from_file"].as<bool>(); }

}  // namespace stockwatch
