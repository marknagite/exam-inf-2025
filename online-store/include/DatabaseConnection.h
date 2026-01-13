#pragma once
#include <pqxx/pqxx>
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>

template<typename T>
class DatabaseConnection {
public:
    DatabaseConnection(const T &connStr);
    std::vector<std::vector<std::string>> executeQuery(const std::string &sql);
    void executeNonQuery(const std::string &sql);
    void beginTransaction();
    void commitTransaction();
    void rollbackTransaction();
    void createFunction(const std::string &sql);
    void createTrigger(const std::string &sql);
    bool getTransactionStatus() const;
    ~DatabaseConnection();
private:
    std::unique_ptr<pqxx::connection> conn;
    std::unique_ptr<pqxx::work> tx;
    bool inTransaction=false;
};
