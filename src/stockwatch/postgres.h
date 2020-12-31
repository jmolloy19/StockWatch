#ifndef STOCKWATCH_POSTGRES_H_
#define STOCKWATCH_POSTGRES_H_

#include <mutex>
#include <string>
#include <pqxx/pqxx>

#include "stockwatch/stock.h"

namespace stockwatch {

class Postgres {
public:
    Postgres() = delete;
    Postgres(const std::string& connection_string);
    ~Postgres() = default;

    bool CreateDatabase();
    bool InsertStock(const Stock& stock);

    private:
    mutable std::mutex mtx_;

    pqxx::connection connection_;
};

}  // namespace stockwatch
#endif  // STOCKWATCH_POSTGRES_H_
