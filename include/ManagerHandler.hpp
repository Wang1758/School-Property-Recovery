#pragma once
#include "hv/HttpServer.h"
#include "hv/hv.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include "SQLDriver.hpp"

class ManagerHandler{
public:
    ManagerHandler();
    ~ManagerHandler();
    void handleGetRequest(HttpService &router, SQLDriver &sql);
    void handlePostRequest(HttpService &router, SQLDriver &sql);
private:
    std::string loadHtmlFile(const std::string &filepath);
};