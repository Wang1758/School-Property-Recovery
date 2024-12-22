#pragma once
#include "hv/HttpServer.h"
#include "hv/hv.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include "SQLDriver.hpp"

class UserHandler{
public:
    UserHandler();
    ~UserHandler();
    void handleGetRequest(HttpService &router);
    void handlePostRequest(HttpService &router, SQLDriver &sql);
private:
    std::string loadHtmlFile(const std::string &filepath);
};