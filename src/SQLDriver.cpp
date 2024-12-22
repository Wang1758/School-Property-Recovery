#include "../include/SQLDriver.hpp"
using namespace sql;

SQLDriver::SQLDriver(std::string host, std::string user, std::string password, std::string database) {
    this->host = host;
    this->user = user;
    this->password = password;
    this->database = database;
    this->connected = false;
    this->driver = get_driver_instance();
}

SQLDriver::~SQLDriver() {
    delete this->stmt;
    delete this->con;
}

void SQLDriver::connect() {
    try {
        con = driver->connect(host, user, password);
        con->setSchema(database);
        connected = true;
        stmt = con->createStatement();
    } catch (SQLException &e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
}

void SQLDriver::execute(std::string query) {
    try {
        stmt->execute(query);
    } catch (SQLException &e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
}

std::vector<std::vector<std::string>> SQLDriver::executeQuery(std::string query)
{
    try {
        ResultSet *res = stmt->executeQuery(query);
        std::vector<std::vector<std::string>> result;
        while (res->next()) {
            std::vector<std::string> row;
            for (int i = 1; i <= res->getMetaData()->getColumnCount(); i++) {
                row.push_back(res->getString(i));
            }
            result.push_back(row);
        }
        delete res;
        return result;
    } catch (SQLException &e) {
        std::cout << "Error: " << e.what() << std::endl;
        return std::vector<std::vector<std::string>>();
    }
}