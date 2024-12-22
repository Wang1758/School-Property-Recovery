#include "hv/HttpServer.h"
#include "hv/hv.h"
#include "../include/UserHandler.hpp"
#include "../include/SQLDriver.hpp"
#include "../include/StudentHandler.hpp"
#include "../include/ManagerHandler.hpp"

int main() {
    HttpService router;
    SQLDriver sql("tcp://192.168.3.173:3306", "wjj", "wjj", "szu_sql");
    UserHandler userHandler;
    StudentHandler studentHandler;
    ManagerHandler managerHandler;

    sql.connect();

    // 设置静态目录为 szu-sql/frontend
    router.Static("/", "frontend");   

    userHandler.handleGetRequest(router);
    userHandler.handlePostRequest(router, sql);
    studentHandler.handlePostRequest(router, sql);
    studentHandler.handleGetRequest(router, sql);
    managerHandler.handlePostRequest(router, sql);
    managerHandler.handleGetRequest(router, sql);

    http_server_t server;
    server.port = 8080;
    server.service = &router;
    http_server_run(&server);
    return 0;
}