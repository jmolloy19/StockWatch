#include "stockwatch/postgres.h"

namespace stockwatch {

Postgres::Postgres(const std::string& connection_string) : connection_(connection_string) {}

bool Postgres::CreateDatabase() {
    pqxx::work transaction(connection_, "Create Database");

    return true;
}

bool Postgres::InsertStock(const Stock& stock) {
    pqxx::work transaction(connection_, "Insert Stock");
    // transaction.exec();

    return true;
}

}  // namespace stockwatch
