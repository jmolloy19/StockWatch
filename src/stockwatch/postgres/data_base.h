#ifndef STOCKWATCH_POSTGRES_DATA_BASE_H_
#define STOCKWATCH_POSTGRES_DATA_BASE_H_

#include <experimental/filesystem>
#include <mutex>
#include <string>
#include <pqxx/pqxx>

#include "stockwatch/stock.h"

namespace stockwatch {
namespace postgres {

class DataBase {
public:
    DataBase() = delete;
    DataBase(const std::string& connection_string);
    ~DataBase() = default;

    void CreateIfNotExists();
    void InsertHighTightFlag(const Stock& stock);

    private:
    static const std::experimental::filesystem::path kMigrationsDir;

    mutable std::mutex mutex_;

    pqxx::connection connection_;
};

}  // namespace postgres
}  // namespace stockwatch
#endif  // STOCKWATCH_POSTGRES_DATA_BASE_H_
