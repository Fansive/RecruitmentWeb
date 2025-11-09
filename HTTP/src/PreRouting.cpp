#include <Server.h>
using httplib::Server;
using httplib::Request;
using httplib::Response;
using std::string;

namespace {
    bool StrStartsWith(const string& str, const string& target) {
        if (target.size() > str.size()) return false;
        int i = 0;
        while (i < target.size() && str[i] == target[i])
            i++;
        return i == target.size();
    }
    bool IsPublicPath(const string& path) {
        return path == "/api/login" || path == "/api/signup" || path == "/"
            || path == "/index.html" || StrStartsWith(path, "/css/") || StrStartsWith(path, "/js/");
    }
}

string HttpHandler::ParseSessionId(const Request& req) {
    auto it = req.headers.find("Cookie");
    if (it == req.headers.end()) return "";

    const string& cookie = it->second; // e.g. "session_id=42; theme=dark"
    string key = "session_id=";
    size_t pos = cookie.find(key);
    if (pos == std::string::npos) return "";

    pos += key.size();
    size_t end = cookie.find(';', pos); // 查找下一个分号或结尾
    if (end == string::npos)
        end = cookie.size();

    return cookie.substr(pos, end - pos);
}
Server::HandlerResponse HttpHandler::PreRoutingHandler(const Request& request, Response& response) {
    if (IsPublicPath(request.path)) {
        return Server::HandlerResponse::Unhandled; //预路由里未处理请求,交由后续路由继续处理
    }
    const string& sessionId = ParseSessionId(request);
    if (Instance.service->IsSessionExists(sessionId)) {
        return Server::HandlerResponse::Unhandled;
    }
    else {
        response.status = 302;                       
        response.set_header("Location", "/login.html"); //重定向
        response.set_content("Redirecting to login...", "text/plain");
        return Server::HandlerResponse::Handled;
    }
}