#include "../include/ManagerHandler.hpp"

ManagerHandler::ManagerHandler() {}

ManagerHandler::~ManagerHandler() {}

std::string ManagerHandler::loadHtmlFile(const std::string &filepath)
{
    std::ifstream file(filepath);
    if (file.is_open())
    {
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

void ManagerHandler::handleGetRequest(HttpService &router, SQLDriver &sql){
    // 处理获取用户列表请求
    router.GET("/manager/getAllstudents", [this,&sql](HttpRequest *req, HttpResponse *resp) {
        try
        {
            std::string query = "SELECT "
                                "USER.account, "
                                "USER.password, "
                                "USER.status, "
                                "USER.role, "
                                "STUDENT.studentID, "
                                "STUDENT.name, "
                                "STUDENT.email, "
                                "STUDENT.phone "
                                "FROM USER "
                                "LEFT JOIN STUDENT ON USER.account = STUDENT.account "
                                "WHERE STUDENT.studentID IS NOT NULL;";
            std::vector<std::vector<std::string>> result = sql.executeQuery(query);
            hv::Json response_body;
            for (auto &row : result)
            {
                hv::Json item;
                item["account"] = row[0];
                item["password"] = row[1];
                item["status"] = row[2];
                item["role"] = row[3];
                item["studentID"] = row[4];
                item["name"] = row[5];
                item["email"] = row[6];
                item["phone"] = row[7];
                response_body.push_back(item);
            }
            resp->body = response_body.dump();
            resp->SetContentType("application/json");
            return 200;
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            return 400;
        }
    });

    // 处理获取申请表请求
    router.GET("/manager/getAllApplications", [this,&sql](HttpRequest *req, HttpResponse *resp) {
        try
        {
            std::string query = "SELECT "
                                "claims.claimsID, "
                                "claims.itemID, "
                                "claims.account, "
                                "claims.gettime, "
                                "claims.isdispose, "
                                "ITEM.itemname, "
                                "ITEM.itemdesctiption, "
                                "ITEM.place, "
                                "ITEM.time, "
                                "STUDENT.studentID, "
                                "STUDENT.name, "
                                "STUDENT.email, "
                                "STUDENT.phone "
                                "FROM "
                                "claims "
                                "JOIN ITEM ON claims.itemID = ITEM.itemID "
                                "JOIN STUDENT ON claims.account = STUDENT.account;";
            std::vector<std::vector<std::string>> result = sql.executeQuery(query);
            hv::Json response_body;
            for (auto &row : result)
            {
                if(row[4] == "1") continue;
                hv::Json item;
                item["claimsID"] = row[0];
                item["itemID"] = row[1];
                item["account"] = row[2];
                item["gettime"] = row[3];
                item["isdispose"] = row[4];
                item["itemname"] = row[5];
                item["itemdesctiption"] = row[6];
                item["place"] = row[7];
                item["time"] = row[8];
                item["studentID"] = row[9];
                item["name"] = row[10];
                item["email"] = row[11];
                item["phone"] = row[12];
                response_body.push_back(item);
            }
            resp->body = response_body.dump();
            resp->SetContentType("application/json");
            return 200;
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            return 400;
        }
    });

    // 获取所有的投诉信息
    router.GET("/manager/getAllComplaints", [this,&sql](HttpRequest *req, HttpResponse *resp) {
        try
        {
            std::string query = "SELECT "
                                "complaints.complaintID, "
                                "complaints.account, "
                                "complaints.complainttime, "
                                "complaints.complaintreason, "
                                "complaints.isdispose, "
                                "STUDENT.studentID, "
                                "STUDENT.name, "
                                "STUDENT.email, "
                                "STUDENT.phone "
                                "FROM "
                                "complaints "
                                "JOIN STUDENT ON complaints.account = STUDENT.account;";
            std::vector<std::vector<std::string>> result = sql.executeQuery(query);
            hv::Json response_body;
            for (auto &row : result)
            {
                if(row[4] == "1") continue;
                hv::Json item;
                item["complaintID"] = row[0];
                item["account"] = row[1];
                item["complainttime"] = row[2];
                item["complaintreason"] = row[3];
                item["isdispose"] = row[4];
                item["studentID"] = row[5];
                item["name"] = row[6];
                item["email"] = row[7];
                item["phone"] = row[8];
                response_body.push_back(item);
            }
            resp->body = response_body.dump();
            resp->SetContentType("application/json");
            return 200;
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            return 400;
        }
    });
}

void ManagerHandler::handlePostRequest(HttpService &router, SQLDriver &sql){
    // 处理删除用户请求
    router.POST("/manager/deleteStudent", [this,&sql](HttpRequest *req, HttpResponse *resp) {
        try
        {
            hv::Json request_body = hv::Json::parse(req->body);
            std::string account = request_body["account"].get<std::string>();
            std::string query = "call delete_student_info('" + account + "');";
            sql.execute(query);
            resp->body = "{\"status\":\"200\"}";
            resp->SetContentType("application/json");
            return 200;
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            return 400;
        }
    });

    // 处理禁用用户请求
    router.POST("/manager/disableStudent", [this,&sql](HttpRequest *req, HttpResponse *resp) {
        try
        {
            hv::Json request_body = hv::Json::parse(req->body);
            std::string account = request_body["account"].get<std::string>();
            std::string query = "UPDATE USER SET status = '禁用' WHERE account = '" + account + "';";
            sql.execute(query);
            resp->body = "{\"status\":\"200\"}";
            resp->SetContentType("application/json");
            return 200;
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            return 400;
        }
    });

    // 处理启用用户请求
    router.POST("/manager/cancelDisable", [this,&sql](HttpRequest *req, HttpResponse *resp) {
        try
        {
            hv::Json request_body = hv::Json::parse(req->body);
            std::string account = request_body["account"].get<std::string>();
            std::string query = "UPDATE USER SET status = '正常' WHERE account = '" + account + "';";
            sql.execute(query);
            resp->body = "{\"status\":\"200\"}";
            resp->SetContentType("application/json");
            return 200;
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            return 400;
        }
    });

    // 处理同意申请请求
    router.POST("/manager/agreeApplication", [this,&sql](HttpRequest *req, HttpResponse *resp) {
        try
        {
            hv::Json request_body = hv::Json::parse(req->body);
            std::string claimsID = request_body["claimsID"].get<std::string>();
            std::string query = "UPDATE claims SET isdispose = '1' WHERE claimsID = '" + claimsID + "';";
            sql.execute(query);
            resp->body = "{\"status\":\"200\"}";
            resp->SetContentType("application/json");
            return 200;
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            return 400;
        }
    });

    // 处理拒绝申请请求
    router.POST("/manager/rejectApplication", [this,&sql](HttpRequest *req, HttpResponse *resp) {
        try
        {
            hv::Json request_body = hv::Json::parse(req->body);
            std::string claimsID = request_body["claimsID"].get<std::string>();
            std::string query = "call delete_claim_info('" + claimsID + "');";
            sql.execute(query);
            resp->body = "{\"status\":\"200\"}";
            resp->SetContentType("application/json");
            return 200;
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            return 400;
        }
    });

    // 处理投诉请求
    router.POST("/manager/processComplaint", [this,&sql](HttpRequest *req, HttpResponse *resp) {
        try
        {
            hv::Json request_body = hv::Json::parse(req->body);
            std::string complaintID = request_body["complaintID"].get<std::string>();
            std::string opinion = request_body["opinion"].get<std::string>();
            std::string account = request_body["account"].get<std::string>();
            std::string time = request_body["time"].get<std::string>();
            std::string opinionID = account + time;
            std::string query = "insert into opinion values('" + opinionID + "','" + account + "','" + complaintID + "','" + opinion + "','" + time + "');";
            sql.execute(query);
            resp->body = "{\"status\":\"200\"}";
            resp->SetContentType("application/json");
            return 200;
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            return 400;
        }
    });
}