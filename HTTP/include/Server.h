/*Author: xzy*/
#pragma once
#include <httplib.h>
#include <IService.h>
#include <IFileLoader.h>
#include <memory>

class HttpHandler final {
public:
	constexpr static auto IP = "127.0.0.1";
	constexpr static auto Port = 8081;
	static std::string GetURL() {
		return std::string("http://").append(IP).append(":").append(std::to_string(Port));
	}
	static HttpHandler Instance;
	void RegisterRoutingAndRun();

private:
	httplib::Server server;
	std::unique_ptr<IService> service;

	HttpHandler(std::unique_ptr<IService>&& serviceImpl) : service(std::move(serviceImpl)){
	}
	~HttpHandler() {
		server.stop();
	}
	HttpHandler(const HttpHandler&) = delete;
	HttpHandler operator=(const HttpHandler&) = delete;
	HttpHandler(HttpHandler&&) = delete;
	HttpHandler operator=(HttpHandler&&) = delete;

	static std::string ParseSessionId(const httplib::Request& req);
	static httplib::Server::HandlerResponse PreRoutingHandler(const httplib::Request&, httplib::Response&);
	static void LoginHandler(const httplib::Request& request, httplib::Response& response);
	static void SignupHandler(const httplib::Request& request, httplib::Response& response);
	static void GetMeHandler(const httplib::Request& request, httplib::Response& response);
	static void GetJobHunterInfoHandler(const httplib::Request& request, httplib::Response& response);
	static void UpdateHunterInfoHandler(const httplib::Request& request, httplib::Response& response);
	static void GetJobApplicationsByJobHunterHandler(const httplib::Request& request, httplib::Response& response);
	static void GetCompanyByIdHandler(const httplib::Request& request, httplib::Response& response);
	static void UpdateCompanyInfoHandler(const httplib::Request& request, httplib::Response& response);
	static void SearchJobsHandler(const httplib::Request& request, httplib::Response& response);
	static void CreateJobHandler(const httplib::Request& request, httplib::Response& response);
	static void ApplyForJobHandler(const httplib::Request& request, httplib::Response& response);
	static void GetJobApplicationsByCompanyHandler(const httplib::Request& request, httplib::Response& response);
	static void UpdateJobApplicationStatusHandler(const httplib::Request& request, httplib::Response& response);
	static void GetPendingCompaniesEditsHandler(const httplib::Request& request, httplib::Response& response);
	static void GetPendingJobsEditsHandler(const httplib::Request& request, httplib::Response& response);
	static void CheckPendingHandler(const httplib::Request& request, httplib::Response& response);
};
