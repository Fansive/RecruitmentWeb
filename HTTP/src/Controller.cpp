#include <Server.h>
#include <json.hpp>
#include <Logger.h>

using nlohmann::json;
using httplib::Response;
using httplib::Request;
using std::string;

namespace {
	void SetResponse(Response& resp, int status, const json& jsonObj) {
		resp.status = status;
		resp.set_content(jsonObj.dump(), "application/json");
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
	LogReq(request);
	auto jsonObj = json::parse(request.body);
	string id = jsonObj.at("userId"), pwd = jsonObj.at("password"), role = jsonObj.at("role");
	
	bool success;
	auto dto = Instance.service->Signup(id, pwd, role, success);
	switch (success) {
		case true:
			SetResponse(response, 200, { 
				{"id", dto.id},
				{"userId",dto.userId} 
			});
			break;
		case false:
			SetResponse(response, 401, {});
			break;
	}
}
void HttpHandler::LoginHandler(const Request& request, Response& response) {
	LogReq(request);
	auto jsonObj = json::parse(request.body);
	string id = jsonObj.at("userId"), pwd = jsonObj.at("password"), role = jsonObj.at("role");

	string sessionId;
	switch (Instance.service->Login(id, pwd, role, sessionId).success) {
		case true:
			SetResponse(response, 200, {});
			response.set_header("Set-Cookie", "session_id=" + sessionId + "; HttpOnly; Path=/");
			break;
		case false:
			SetResponse(response, 401, {});
			break;
		default: assert(false);
	}
}
