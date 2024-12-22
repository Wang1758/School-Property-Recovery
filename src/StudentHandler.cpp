#include "../include/StudentHandler.hpp"

StudentHandler::StudentHandler() {}

StudentHandler::~StudentHandler() {}

std::string StudentHandler::loadHtmlFile(const std::string &filepath){
    std::ifstream file(filepath);
    if (file.is_open()){
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }
    else{
        std::cerr << "Failed to open file: " << filepath << std::endl;
    }
    return "";
}

void StudentHandler::handlePostRequest(HttpService &router, SQLDriver &sql) {
    // 设置学生修改个人信息接口
    router.POST("/student/profile", [this, &sql](HttpRequest* req, HttpResponse* resp) {
        hv::Json request_body = hv::Json::parse(req->body);
        std::string account = request_body["account"].get<std::string>();
        std::string name = request_body["name"].get<std::string>();
        std::string email = request_body["email"].get<std::string>();
        std::string phone = request_body["phone"].get<std::string>();

        // 更新学生信息
        std::string sql_query = "update student set name = '" + name + "', email = '" + email + "', phone = '" + phone + "' where account = '" + account + "'";
        sql.execute(sql_query);

        request_body.clear();
        request_body["status"] = "200";
        resp->body = request_body.dump();
        return 200;
    });

    // 设置学生提交物品接口
    router.POST("/student/postItem", [this, &sql](HttpRequest* req, HttpResponse* resp) {
        try {
            hv::Json request_body = hv::Json::parse(req->body);
            std::string account = request_body["account"].get<std::string>();
            std::string studentID = request_body["studentID"].get<std::string>();
            std::string itemtype = request_body["itemtype"].get<std::string>();
            std::string itemname = request_body["itemname"].get<std::string>();
            std::string itemdesctiption = request_body["itemdesctiption"].get<std::string>();
            std::string place = request_body["place"].get<std::string>();
            std::string time = request_body["time"].get<std::string>();
            std::string thistime = request_body["thistime"].get<std::string>();
            // itemid是account和thistime的组合
            std::string itemid = account + " : " + thistime;
            bool isget = false;

            // 插入物品信息
            std::string sql_query = "insert into item values('" + itemid + "', '" + account + "', '" + studentID + "', '" + 
            itemtype + "', '" + itemname + "', '" + itemdesctiption + "', '" + place + "', '" + time + "', " + std::to_string(isget) + ")";
            sql.execute(sql_query);

            request_body.clear();
            request_body["status"] = "200";
            resp->body = request_body.dump();
            return 200;
        }
        catch(const std::exception& e) {
            std::cerr << e.what() << '\n';
            return 400;
        }
    });

    // 设置学生认领物品接口
    router.POST("/student/submitApplication", [this, &sql](HttpRequest* req, HttpResponse* resp) {
        try{
            hv::Json request_body = hv::Json::parse(req->body);
            std::string account = request_body["account"].get<std::string>();
            std::string itemid = request_body["itemid"].get<std::string>();
            std::string gettime = request_body["gettime"].get<std::string>();
            bool isdispose = false;
            std::string claimsID = "领回表"+gettime;
    
            // 插入认领信息
            std::string sql_query = "insert into claims values('" + claimsID + "', '" + itemid + "', '"+ account + 
            "', '" +gettime + "', " + std::to_string(isdispose) + ")";
            sql.execute(sql_query);

            request_body.clear();
            request_body["status"] = "200";
            resp->body = request_body.dump();
            return 200;
        }
        catch(const std::exception& e){
            std::cerr << e.what() << '\n';
            return 400;
        }
    });

    // 获取当前物品的所有认领信息接口
    router.POST("/student/getApplicationsForItem", [this, &sql](HttpRequest *req, HttpResponse *resp)
    {
        try {
            hv::Json request_body = hv::Json::parse(req->body);
            std::string itemid = request_body["itemid"].get<std::string>();
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
                                "FROM claims "
                                    "JOIN ITEM ON claims.itemID = ITEM.itemID "
                                    "JOIN STUDENT ON claims.account = STUDENT.account "
                                "WHERE claims.itemID = '" +
                                itemid + "';";


            std::vector<std::vector<std::string>> result = sql.executeQuery(query);
            hv::Json response_body;
            for (auto &row : result){
                hv::Json item;
                item["account"] = row[2];
                item["name"] = row[10];
                item["email"] = row[11];
                item["phone"] = row[12];
                item["claimsID"] = row[0];
                response_body.push_back(item);
            }
            resp->body = response_body.dump();
            return 200;
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            return 400;
        } 
    });

    // 用户提交投诉接口
    router.POST("/student/submitComplaint", [this, &sql](HttpRequest *req, HttpResponse *resp)
    {
        try {
            hv::Json request_body = hv::Json::parse(req->body);
            std::string account = request_body["account"].get<std::string>();
            std::string claimsID = request_body["claimsID"].get<std::string>();
            std::string complaintreason = request_body["complaintReason"].get<std::string>();
            std::string complainttype = request_body["complaintCategory"].get<std::string>();
            std::string complainttime = request_body["time"].get<std::string>();
            std::string isdispose = "0";
            std::string complaintID = "投诉表"+complainttime;

            // 插入投诉信息
            std::string sql_query = "insert into complaints values('" + complaintID + "', '" + claimsID + "', '"+ account + 
            "', '" +complaintreason + "', '" + complainttype + "', '" + complainttime + "', " + isdispose + ")";
            sql.execute(sql_query);

            request_body.clear();
            request_body["status"] = "200";
            resp->body = request_body.dump();
            return 200;
        }
        catch(const std::exception& e){
            std::cerr << e.what() << '\n';
            return 400;
        } 
    });
}

void StudentHandler::handleGetRequest(HttpService &router, SQLDriver &sql) {
    // 获取没有被认领的所有的物品信息接口
    router.GET("/student/getAllItems", [this, &sql](HttpRequest* req, HttpResponse* resp) {
        try
        {
            std::string query = "select * from ITEM where isget = false";
            std::vector<std::vector<std::string>> result = sql.executeQuery(query);
            hv::Json response_body;
            for (auto &row : result){
                hv::Json item;
                item["itemid"] = row[0];
                item["studentID"] = row[2];
                item["itemtype"] = row[3];
                item["itemname"] = row[4];
                item["itemdesctiption"] = row[5];
                item["place"] = row[6];
                item["time"] = row[7];
                response_body.push_back(item);
            }
            resp->body = response_body.dump();
            return 200;
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            return 400;
        }
    });
}