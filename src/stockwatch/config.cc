#include "stockwatch/config.h"

#include "glog/logging.h"

namespace stockwatch {

Config::Config(const cxxopts::ParseResult& args)
    : finnhub_api_key_(args["finnhub_api_key"].as<std::string>()),
      write_to_file_(args["write_to_file"].as<bool>()),
      read_from_file_(args["read_from_file"].as<bool>()),
      db_name_(args["db_name"].as<std::string>()),
      db_user_(args["db_user"].as<std::string>()),
      db_password_(args["db_password"].as<std::string>()),
      db_address_(args["db_address"].as<std::string>()),
      db_port_(args["db_port"].as<std::string>()) {}

const std::experimental::filesystem::path& Config::JsonsDir() const { return jsons_dir_; }

const std::string& Config::FinnhubApiKey() const { return finnhub_api_key_; }

void Config::Log() const {
    LOG(INFO) << "\n----- StockWatch Configuration -----"
              << "\nLog Verbosity:   " << FLAGS_v
              << "\nWrite To File:   " << (write_to_file_ ? "true\nWrite File Dir:  " + jsons_dir_.string() : "false")
              << "\nRead From File:  " << (read_from_file_ ? "true\nRead File Dir:   " + jsons_dir_.string() : "false")
              << "\nDB Name:         " << db_name_
              << "\nDB User:         " << db_user_
              << "\nDB Address:      " << db_address_
              << "\nDB Port:         " << db_port_
              << "\n------------------------------------";
}

std::string Config::DbConnectionString() const {
    std::string connection_string("dbname = " + db_name_);
    connection_string.append(" user = " + db_user_);
    connection_string.append(" password = " + db_password_);
    connection_string.append(" hostaddr = " + db_address_);
    connection_string.append(" port = " + db_port_);

    return connection_string;
}

bool Config::IsWriteToFile() const {
    if (read_from_file_ and write_to_file_) {
        LOG_EVERY_N(WARNING, 100)
            << "NOT writing to file!!!!! Both \'read_from_file\' and \'write_to_file\' options set to true. "
               "No point writing to file if reading from it.";
        return false;
    }

    return write_to_file_;
}

bool Config::IsReadFromFile() const { return read_from_file_; }

}  // namespace stockwatch
