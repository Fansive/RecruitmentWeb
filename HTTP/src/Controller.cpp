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
	int GetId(const Request& request) {
		return atoi(request.matches[1].str().c_str());
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

void HttpHandler::GetMeHandler(const httplib::Request& request, httplib::Response& response) {
	auto dto = Instance.service->GetMe(ParseSessionId(request));
	SetResponse(response, 200, {
		{"id", dto.id},
		{"userId" , dto.userId},
		{"role" , dto.role},
		{"email" , dto.email}
	});
}
void HttpHandler::GetJobHunterInfoHandler(const httplib::Request& request, httplib::Response& response) {
	int id = GetId(request);
	auto dto = Instance.service->GetJobHunterInfo(id);
	SetResponse(response, 200, {
		{"id", dto.id},
		{"userId", dto.userId},
		{"createdAt", dto.createdAt},
		{"info", {
			{"userId", dto.info.userId},
			{"passwordHash", dto.info.passwordHash},
			{"email", dto.info.email},
			{"role", dto.info.role},
			{"name", dto.info.name},
			{"address", dto.info.address},
			{"gender", dto.info.gender},
			{"phoneNumber", dto.info.phoneNumber},
			{"degree", dto.info.degree},
			{"graduationYear", dto.info.graduationYear},
			{"school", dto.info.school},
			{"major", dto.info.major},
			{"resumeUrl", dto.info.resumeUrl}
		}}
	});
}
void HttpHandler::UpdateHunterInfoHandler(const httplib::Request& request, httplib::Response& response) {
	auto jsonObj = json::parse(request.body);
	JobHunter newInfo;
	newInfo.email = jsonObj.at("email");
	newInfo.address = jsonObj.at("address");
	newInfo.phoneNumber = jsonObj.at("phoneNumber");
	newInfo.gender = jsonObj.at("gender");
	newInfo.degree = jsonObj.at("degree");
	newInfo.graduationYear = jsonObj.at("graduationYear");
	newInfo.school = jsonObj.at("school");
	newInfo.major = jsonObj.at("major");
	newInfo.resumeUrl = jsonObj.at("resumeUrl");
	int id = GetId(request);
	auto dto = Instance.service->UpdateHunterInfo(id, newInfo);
	SetResponse(response, 200, {
		{"success", dto.success}
	});
}
void HttpHandler::GetJobApplicationsByJobHunterHandler(const httplib::Request& request, httplib::Response& response) {
	int id = GetId(request);
	json retJson;
	for (auto& dto : Instance.service->GetJobApplicationsByJobHunter(id)) {
		retJson.push_back({
			{"id", dto.id},
			{"jobId", dto.jobId},
			{"appliedAt", dto.appliedAt},
			{"status", dto.status},
			{"jobInfo", {
				{"id", dto.jobInfo.id},
				{"title", dto.jobInfo.title},
				{"companyName", dto.jobInfo.companyName},
				{"location", dto.jobInfo.location},
				{"salary", dto.jobInfo.salary},
				{"description", dto.jobInfo.description},
				{"degreeRequirement", dto.jobInfo.degreeRequirement}
			}}
		});
	}
	SetResponse(response, 200, retJson);
}
void HttpHandler::GetCompanyByIdHandler(const httplib::Request& request, httplib::Response& response) {
	int id = GetId(request);
	auto dto = Instance.service->GetCompanyById(id);
	SetResponse(response, 200, {
		{"id", dto.id},
		{"userId", dto.userId},
		{"createdAt", dto.createdAt},
		{"companyInfo", {
			{"id", dto.companyInfo.id},
			{"userId", dto.companyInfo.userId},
			{"passwordHash", dto.companyInfo.passwordHash},
			{"email", dto.companyInfo.email},
			{"role", dto.companyInfo.role},
			{"name", dto.companyInfo.name},
			{"address", dto.companyInfo.address},
			{"companyType", dto.companyInfo.companyType},
			{"scale", dto.companyInfo.scale},
			{"contactInfo", dto.companyInfo.contactInfo},
			{"description", dto.companyInfo.description}
		}}
	});
}
void HttpHandler::UpdateCompanyInfoHandler(const httplib::Request& request, httplib::Response& response) {
	int id = GetId(request);
	auto jsonObj = json::parse(request.body);
	Company newInfo;
	newInfo.email = jsonObj.at("email");
	newInfo.address = jsonObj.at("address");
	newInfo.contactInfo = jsonObj.at("contactInfo");
	newInfo.scale = jsonObj.at("scale");
	newInfo.companyType = jsonObj.at("companyType");
	newInfo.description = jsonObj.at("description");
	auto dto = Instance.service->UpdateCompanyInfo(id, newInfo);
	SetResponse(response, 200, {
		{"success", dto.success}
	});
}
void HttpHandler::SearchJobsHandler(const httplib::Request& request, httplib::Response& response) {
	string keyword = request.get_param_value("keyword");
	string location = request.get_param_value("location");
	json retJson;
	for (auto& dto : Instance.service->SearchJobs(keyword, location)) {
		retJson.push_back({
			{"companyId", dto.companyId},
			{"createdAt", dto.createdAt},
			{"info", {
				{"id", dto.info.id},
				{"title", dto.info.title},
				{"companyName", dto.info.companyName},
				{"location", dto.info.location},
				{"salary", dto.info.salary},
				{"description", dto.info.description},
				{"degreeRequirement", dto.info.degreeRequirement}
			}}
		});
	}
	SetResponse(response, 200, retJson);
}
void HttpHandler::CreateJobHandler(const httplib::Request& request, httplib::Response& response) {
	auto jsonObj = json::parse(request.body);
	JobInfo newInfo;
	newInfo.title = jsonObj.at("title");
	newInfo.description = jsonObj.at("description");
	newInfo.location = jsonObj.at("location");
	newInfo.salary = jsonObj.at("salary");
	newInfo.degreeRequirement = jsonObj.at("degreeRequirement");
	int retId = Instance.service->CreateJob(newInfo);
	SetResponse(response, 200, { "id",retId });
}
void HttpHandler::ApplyForJobHandler(const httplib::Request& request, httplib::Response& response) {
	int id = GetId(request);
	auto jsonObj = json::parse(request.body);
	string jobHunterIdStr = jsonObj.at("jobHunterId");
	auto dto = Instance.service->ApplyForJob(id, std::stoi(jobHunterIdStr));
	SetResponse(response, 200, {
		{"id", dto.id},
		{"appliedAt", dto.appliedAt},
		{"status", dto.status},
	});
}
void HttpHandler::GetJobApplicationsByCompanyHandler(const httplib::Request& request, httplib::Response& response) {
	int id = GetId(request);
	json retJson;
	for (auto& dto : Instance.service->GetJobApplicationsByCompany(id)) {
		retJson.push_back({
			{"id", dto.id},
			{"jobId", dto.jobId},
			{"appliedAt", dto.appliedAt},
			{"status", dto.status},
			{"jobInfo", {
				{"id", dto.jobInfo.id},
				{"title", dto.jobInfo.title},
				{"companyName", dto.jobInfo.companyName},
				{"location", dto.jobInfo.location},
				{"salary", dto.jobInfo.salary},
				{"description", dto.jobInfo.description},
				{"degreeRequirement", dto.jobInfo.degreeRequirement}
			}}
		});
	}
	SetResponse(response, 200, retJson);
}
void HttpHandler::UpdateJobApplicationStatusHandler(const httplib::Request& request, httplib::Response& response) {
	int id = GetId(request);
	auto jsonObj = json::parse(request.body);
	auto dto = Instance.service->UpdateJobApplicationStatus(id, jsonObj.at("status"));
	SetResponse(response, 200, {
		{"success", dto.success}
	});
}
void HttpHandler::GetPendingCompaniesEditsHandler(const httplib::Request& request, httplib::Response& response) {
	json retJson;
	for (auto& dto : Instance.service->GetPendingCompaniesEdits()) {
		json companyData = json::parse(dto.reviewDataJson);

		retJson.push_back({
			{"id", dto.id},
			{"companyId", dto.companyId},
			{"companyName", dto.companyName},
			{"createdAt", dto.createdAt},
			{"data", {
				{"email", companyData.at("email")},
				{"address", companyData.at("address")},
				{"contactInfo", companyData.at("contactInfo")},
				{"scale", companyData.at("scale")},
				{"companyType", companyData.at("companyType")},
				{"description", companyData.at("description")}
			}}
		});
	}
	SetResponse(response, 200, retJson);
}
void HttpHandler::GetPendingJobsEditsHandler(const httplib::Request& request, httplib::Response& response) {
	json retJson;
	for (auto& dto : Instance.service->GetPendingJobsEdits()) {
		json jobData = json::parse(dto.reviewDataJson);

		retJson.push_back({
			{"id", dto.id},
			{"companyId", dto.companyId},
			{"companyName", dto.companyName},
			{"createdAt", dto.createdAt},
			{"data", {
				{"title", jobData.at("title")},
				{"description", jobData.at("description")},
				{"location", jobData.at("location")},
				{"salary", jobData.at("salary")},
				{"degreeRequirement", jobData.at("degreeRequirement")}
			}}
		});
	}

	SetResponse(response, 200, retJson);
}
void HttpHandler::CheckPendingHandler(const httplib::Request& request, httplib::Response& response) {
	int id = GetId(request);
	auto jsonObj = json::parse(request.body);
	string type = jsonObj.at("type"), status = jsonObj.at("status");
	string pendingReviewIdStr = jsonObj.at("pendingReviewId");
	auto dto = Instance.service->CheckPending(type, status == "approved", std::stoi(pendingReviewIdStr));
	SetResponse(response, 200, {
		{"success", dto.success}
	});
}