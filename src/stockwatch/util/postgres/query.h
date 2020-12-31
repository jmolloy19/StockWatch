#ifndef STOCKWATCH_UTIL_POSTGRES_QUERY_H_
#define STOCKWATCH_UTIL_POSTGRES_QUERY_H_

#include <sstream>
#include <string>

namespace stockwatch {
namespace util {
namespace postgres {

class Query {
    public:
    std::string ToString() const { return QueryStream().str(); }

    template <class T>
    Query& From(T&& arg) {
        QueryStream() << " FROM " << arg;
        return *this;
    }

    template <class... Ts>
    Query& From(Ts&&... args) {
        QueryStream() << " FROM " << std::forward<Ts&&>(args)...;
        return *this;
    }

    template <class T>
    Query& Where(T&& arg) {
        QueryStream() << " WHERE " << arg;
        return *this;
    }

    template <class... Ts>
    Query& Where(Ts&&... args) {
        QueryStream() << " WHERE " << std::forward<Ts&&>(args)...;
        return *this;
    }

    protected:
     virtual std::stringstream& QueryStream() = 0;
     virtual const std::stringstream& QueryStream() const = 0;
};

class Select : public Query {
    public:
    Select() = delete;

    template <class T>
    Select(T&& arg) {
        query_ << "SELECT " << arg;
    }

    template <class... Ts>
    Select(Ts&&... args) {
        query_ << "SELECT " << std::forward<Ts&&>(args)...;
    }

    protected:
    std::stringstream& QueryStream() override { return query_; }
    const std::stringstream& QueryStream() const override { return query_; }

    private:
    std::stringstream query_;
};
}  // namespace postgres
}  // namespace util
}  // namespace stockwatch

#endif  // STOCKWATCH_UTIL_POSTGRES_QUERY_H_
