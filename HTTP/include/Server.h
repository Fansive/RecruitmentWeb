/*Author: xzy*/
#pragma once
#include <httplib.h>

class HttpHandler final {
public:
	constexpr static auto IP = "0.0.0.0";
	constexpr static auto Port = 8081;
	static std::string GetURL() {
		std::string str = "http://";
		str += IP;
		str += ":";
		str += std::to_string(Port);
		str += "\n";
		return str;
	}
	static HttpHandler& Instance() {
		static HttpHandler instance;
		return instance;
	}
	void RegisterRoutingAndRun();

private:
	httplib::Server server;
	void RegisterAccountManagementRouting();
	void RegisterPageRouting();

	HttpHandler() = default;
	~HttpHandler() {
		server.stop();
	}
	HttpHandler(const HttpHandler&) = delete;
	HttpHandler operator=(const HttpHandler&) = delete;
	HttpHandler(HttpHandler&&) = delete;
	HttpHandler operator=(HttpHandler&&) = delete;
};

