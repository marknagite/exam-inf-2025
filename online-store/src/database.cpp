#include "DatabaseConnection.h"
#include <sstream>

template<typename T>
DatabaseConnection<T>::DatabaseConnection(const T &connStr){
    conn = std::make_unique<pqxx::connection>(connStr);
    if(!conn->is_open()) throw std::runtime_error("DB open failed");
}

template<typename T>
std::vector<std::vector<std::string>> DatabaseConnection<T>::executeQuery(const std::string &sql){
    pqxx::work w(*conn);
    pqxx::result r = w.exec(sql);
    std::vector<std::vector<std::string>> out;
    for(auto row: r){
        std::vector<std::string> cols;
        for(size_t i=0;i<row.size();++i) cols.push_back(row[i].c_str()==nullptr?"":row[i].as<std::string>());
        out.push_back(std::move(cols));
    }
    return out;
}

template<typename T>
void DatabaseConnection<T>::executeNonQuery(const std::string &sql){
    if(inTransaction && tx){
        tx->exec(sql);
    } else {
        pqxx::work w(*conn);
        w.exec(sql);
        w.commit();
    }
}

template<typename T>
void DatabaseConnection<T>::beginTransaction(){
    if(inTransaction) throw std::runtime_error("Already in transaction");
    tx = std::make_unique<pqxx::work>(*conn);
    inTransaction = true;
}

template<typename T>
void DatabaseConnection<T>::commitTransaction(){
    if(!inTransaction || !tx) throw std::runtime_error("No transaction");
    tx->commit();
    tx.reset();
    inTransaction=false;
}

template<typename T>
void DatabaseConnection<T>::rollbackTransaction(){
    if(!inTransaction || !tx) return;
    tx->abort();
    tx.reset();
    inTransaction=false;
}

template<typename T>
void DatabaseConnection<T>::createFunction(const std::string &sql){
    executeNonQuery(sql);
}

template<typename T>
void DatabaseConnection<T>::createTrigger(const std::string &sql){
    executeNonQuery(sql);
}

template<typename T>
bool DatabaseConnection<T>::getTransactionStatus() const{
    return inTransaction;
}

template<typename T>
DatabaseConnection<T>::~DatabaseConnection(){
    if(inTransaction) rollbackTransaction();
    if(conn && conn->is_open()) conn->disconnect();
}

template class DatabaseConnection<std::string>;
