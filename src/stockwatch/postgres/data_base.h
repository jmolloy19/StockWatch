#ifndef STOCKWATCH_POSTGRES_DATA_BASE_H_
#define STOCKWATCH_POSTGRES_DATA_BASE_H_

#include <experimental/filesystem>
#include <mutex>
#include <pqxx/pqxx>
#include <string>
#include <unordered_set>

#include "stockwatch/stock.h"
#include "stockwatch/patterns/pattern.h"

namespace stockwatch {
namespace postgres {

class DataBase {
   public:
    DataBase() = delete;
    DataBase(const std::string& connection_string);
    ~DataBase() = default;

    void Init();
    bool IsCurrentlyWatched(const std::string& symbol) const;
    void InsertNewCandles(const Stock& stock);
    void WatchStock(const Stock& stock, Pattern pattern);
    
    const std::vector<std::pair<std::string, Pattern>>& NewlyWatchedPatterns() const;
    pqxx::result QueryAllWatchedPatterns() const;

   protected:
    pqxx::connection Connect() const;
    void CreateDbIfNotExists() const;
    void InitCurrentlyWatchedStocks();

   private:
    static const std::experimental::filesystem::path kMigrationsDir;
    const std::string connection_string_;

    mutable std::mutex mutex_;

    std::vector<std::pair<std::string, Pattern>> newly_watched_patterns_;
    std::unordered_set<std::string> currently_watched_stocks_;
};

}  // namespace postgres
}  // namespace stockwatch
#endif  // STOCKWATCH_POSTGRES_DATA_BASE_H_
