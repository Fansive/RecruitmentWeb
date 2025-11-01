#include <Server.h>
#include <IAccountManager.h>
#include <json.hpp>

using nlohmann::json;
using httplib::Response;
using httplib::Request;
using std::string;

namespace {
	class MockAccountManager : public IAccountManager {
	public:
		RetType Login(const string& id, const string& password, string &sessionId) override {
			printf("[received login]:id=%s; password=%s\n", id.c_str(), password.c_str());
			sessionId = "42";
			return RetType::Success;
		}
		RetType Signup(const string& id, const string& password, const string& role) override {
			printf("[received signup]:id=%s; password=%s\n", id.c_str(), password.c_str());
			return RetType::Success;
		}
	};

	void SetResponse(Response& resp, int status, const string& json_status, const string& json_message) {
		resp.status = status;
		json result = {
			{"status", json_status},
			{"message", json_message}
		};
		resp.set_content(result.dump(), "application/json");
	}
	void SignupHandler(const Request& request, Response& response) {
		auto jsonObj = json::parse(request.body);
		string id = jsonObj.at("id"), pwd = jsonObj.at("password");
		string role = jsonObj.at("role");

		MockAccountManager accountManagerImpl;
		IAccountManager& accountManager = accountManagerImpl;
		switch (accountManager.Signup(id, pwd, role)) {
			case IAccountManager::RetType::Success:
				SetResponse(response, 200, "ok", "signup success");
				break;
			case IAccountManager::RetType::Signup_Existed:
				SetResponse(response, 401, "failed", "signup failed: existed account");
				break;
			default: assert(false);
		}
	}
	void LoginHandler(const Request& request, Response& response) {
		auto jsonObj = json::parse(request.body);
		string id = jsonObj.at("id"), pwd = jsonObj.at("password");

		MockAccountManager accountManagerImpl;//todo 长期持有
		IAccountManager& accountManager = accountManagerImpl;
		string sessionId;
		switch (accountManager.Login(id, pwd, sessionId)) {
			case IAccountManager::RetType::Success:
				SetResponse(response, 200, "ok", "login success");
				response.set_header("Set-Cookie", 
					"session_id=" + sessionId + "; HttpOnly; Path=/");
				break;
			case IAccountManager::RetType::Login_Unmatch:
				SetResponse(response, 401, "failed", "login failed: unmatched id and password");
				break;
			default: assert(false);
		}
	}
}

void HttpHandler::RegisterAccountManagementRouting() {
	server.Post("/api/signup", SignupHandler);
	server.Post("/api/login", LoginHandler);

}