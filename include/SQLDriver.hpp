#pragma once
#include <cppconn/statement.h>
#include <cppconn/resultset.h>
#include <cppconn/exception.h>
#include <mysql_driver.h>
#include <iostream>
using namespace sql;

class SQLDriver {
public:
    SQLDriver(std::string host, std::string user, std::string password, std::string database);
    ~SQLDriver();
    void connect();
    bool isConnected();
    void execute(std::string query);
    std::vector<std::vector<std::string>> executeQuery(std::string query);

private:
    Driver *driver;
    sql::Connection *con;
    sql::Statement *stmt;
    std::string host;
    std::string user;
    std::string password;
    std::string database;
    bool connected;
};