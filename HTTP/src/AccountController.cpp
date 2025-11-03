#include <Server.h>
#include <json.hpp>

using nlohmann::json;
using httplib::Response;
using httplib::Request;
using std::string;

namespace {
	void SetResponse(Response& resp, int status, const string& json_status, const string& json_message) {
		resp.status = status;
		json result = {
			{"status", json_status},
			{"message", json_message}
		};
		resp.set_content(result.dump(), "application/json");
	}
	void PrintReqHeader(const Request& req) {
		std::cout << "--Request Header--" << "\r\n";
		for (const auto& kvp : req.headers) {
			std::cout << kvp.first << ": " << kvp.second << "\r\n";
		}
		std::cout << std::endl;
	}
}
void HttpHandler::SignupHandler(const Request& request, Response& response) {
	PrintReqHeader(request);
	auto jsonObj = json::parse(request.body);
	string id = jsonObj.at("id"), pwd = jsonObj.at("password"), role = jsonObj.at("role");
	
	switch (Instance.service->Signup(id, pwd, role)) {
		case IService::RetType::Success:
			SetResponse(response, 200, "ok", "signup success");
			break;
		case IService::RetType::Signup_Existed:
			SetResponse(response, 401, "failed", "signup failed: existed account");
			break;
		default: assert(false);
	}
}
void HttpHandler::LoginHandler(const Request& request, Response& response) {
	PrintReqHeader(request);
	auto jsonObj = json::parse(request.body);
	string id = jsonObj.at("id"), pwd = jsonObj.at("password");

	string sessionId;
	switch (Instance.service->Login(id, pwd, sessionId)) {
		case IService::RetType::Success:
			SetResponse(response, 200, "ok", "login success");
			response.set_header("Set-Cookie", "session_id=" + sessionId + "; HttpOnly; Path=/");
			break;
		case IService::RetType::Login_Unmatch:
			SetResponse(response, 401, "failed", "login failed: unmatched id and password");
			break;
		default: assert(false);
	}
}
