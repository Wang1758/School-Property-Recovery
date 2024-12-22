#include "../include/UserHandler.hpp"

UserHandler::UserHandler()
{
}

UserHandler::~UserHandler()
{
}

void UserHandler::handleGetRequest(HttpService &router)
{
    // 设置默认首页
    router.GET("/", [this](HttpRequest* req, HttpResponse* resp) {
        resp->Redirect("/login.html");
        return 302;
    });

    router.GET("/login.html", [this](HttpRequest* req, HttpResponse* resp) {
        resp->body = loadHtmlFile("frontend/login.html");
        resp->headers["Content-Type"] = "text/html";
        return 200;
    });

    router.GET("/register.html", [this](HttpRequest* req, HttpResponse* resp) {
        resp->body = loadHtmlFile("frontend/register.html");
        resp->headers["Content-Type"] = "text/html";
        return 200;
    });

    router.GET("/managerHome.html", [this](HttpRequest* req, HttpResponse* resp) {
        resp->body = loadHtmlFile("frontend/managerHome.html");
        resp->headers["Content-Type"] = "text/html";
        return 200;
    });
}

std::string UserHandler::loadHtmlFile(const std::string &filepath)
{
    std::ifstream file(filepath);
    if (file.is_open()) {
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }
    else
    {
        std::cerr << "Failed to open file: " << filepath << std::endl;
    }
    return "";
}

void UserHandler::handlePostRequest(HttpService &router, SQLDriver &sql)
{
    // 设置登录接口
    router.POST("/user/login", [this, &sql](HttpRequest* req, HttpResponse* resp) {
        hv::Json request_body = hv::Json::parse(req->body);
        std::string username = request_body["username"].get<std::string>();
        std::string password = request_body["password"].get<std::string>();

        // 从数据库中获取所有用户名和密码
        std::string sql_query = "select role, status from user where account = '" + username + "' and password = '" + password + "'";
        std::vector<std::vector<std::string>> result = sql.executeQuery(sql_query);

        hv::Json response_body;
        // 检查用户名和密码
        if(!result.empty()){
            std::string role = result[0][0];
            if(result[0][1] == "禁用"){
                resp->body = "用户已被禁用";
                response_body["status"] = "403";
                return 403;
            }
            if(role == "学生"){
                resp->Redirect("/userHome.html");
            }
            else if(role == "管理员"){
                resp->Redirect("/managerHome.html");
            }
            return 302;
        }else {
            resp->body = "用户名或密码错误";
            return 401;
        }
    });

    // 设置注册接口
    router.POST("/user/register", [this, &sql](HttpRequest* req, HttpResponse* resp) {
        hv::Json request_body = hv::Json::parse(req->body);
        std::string username = request_body["username"].get<std::string>();
        std::string password = request_body["password"].get<std::string>();

        // 检查用户名是否已存在
        std::string sql_query = "select * from user where account = '" + username + "'";
        std::vector<std::vector<std::string>> result = sql.executeQuery(sql_query);
        if(!result.empty()){
            resp->body = "用户名已存在";
            return 409;
        }

        // 插入新用户
        sql_query = "insert into user(account, password, status, role) values('" + username + "', '" + password + "', '" + "正常" + "', '" + "学生" + "')";
        sql.execute(sql_query);
        
        request_body.clear();
        request_body["status"] = "200";
        resp->body = request_body.dump();
        return 200;
    });

    // 设置查询用户接口
    router.POST("/user/check_username", [this, &sql](HttpRequest* req, HttpResponse* resp) {
        hv::Json request_body = hv::Json::parse(req->body);
        std::string username = request_body["username"].get<std::string>();

        // 检查用户名是否已存在
        std::string sql_query = "select * from user where account = '" + username + "'";
        std::vector<std::vector<std::string>> result = sql.executeQuery(sql_query);
        if(!result.empty()){
            resp->body = "用户名已经存在";
            return 409;
        }else{
            resp->body = "用户名可用";
            return 200;
        }
    });
}