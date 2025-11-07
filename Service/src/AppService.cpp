#pragma once
#include <string>
#include <vector>
#include <random>
#include <algorithm>
#include "IService.h"
#include "IPersistent.h"
#include "DomainModels.h"
#include "DTO.h"
#include <unordered_map>
#include <string>
#include <mutex>
#include "DTO.h"
#include <chrono>
#include <sstream>
#include "IPersistent.h"
#include <iostream>
using string = std::string;

class AppService : public IService, public IPersistent {

private:
	std::unordered_map<std::string, std::string> userPasswords_; // userId -> password
	std::unordered_map<std::string, std::string> sessionUsers_;  // sessionId -> userId
	std::mutex mu_;


public:
	std::string MakeSessionId(const std::string& userId) {
		auto now = std::chrono::steady_clock::now().time_since_epoch().count();
		std::ostringstream oss;
		oss << userId << "-" << now;
		return oss.str();
	}


	bool IsSessionExists(const string& sessionId) override
	{
		std::lock_guard<std::mutex> lk(mu_);
		return sessionUsers_.find(sessionId) != sessionUsers_.end();

	}
	StatusDTO Login(const std::string& userId, const std::string& password,
		const std::string& role, std::string& sessionId) override {
		std::lock_guard<std::mutex> lk(mu_);

		// 如果 session 已存在，则直接成功
		if (IsSessionExists(sessionId)) {
			return { true };
		}

		// 检查账号密码
		auto it = userPasswords_.find(userId);
		if (it != userPasswords_.end() && it->second == password) {
			sessionId = MakeSessionId(userId);
			sessionUsers_[sessionId] = userId;
			return { true };
		}
		return { false };
	}
	
	SignupDTO Signup(const std::string& userId, const std::string& password,
		const std::string& role, bool& success) override {
		std::lock_guard<std::mutex> lk(mu_);
		if (userPasswords_.count(userId)) {
			success = false; // 已存在
			return {};
		}
		userPasswords_[userId] = password;
		success = true;
		int id = CreateUser(userId, role);
		return { std::to_string(id), userId };
	}
	UserInfoDTO GetMe(const string& sessionId) override
	{
		std::lock_guard<std::mutex> lk(mu_);

		auto it = sessionUsers_.find(sessionId);
		if (it == sessionUsers_.end()) {
			return {};  // session不存在，返回空结构体
		}
		std::string userId = it->second;
		int id= GetUserIdByUserName(userId);
		if (id==NULL) {
			return {}; // 用户在数据库不存在，返回空
		}
		UserInfoDTO info = GetUserInfo(id);
		return info;
	}


	virtual JobHunterDTO GetJobHunterInfo(int id)  override
	{
		try {
			return IPersistent::GetJobHunterInfo(id);
		}
		catch (const std::exception& e) {
			//
			return {};
		}

	}
	virtual StatusDTO UpdateHunterInfo(int id, const JobHunter& newInfo) override
	{
		try {
			IPersistent::UpdateHunterInfo(id, newInfo);
			return { true };
		}
		catch (const std::exception& e) {
			// 处理异常，返回失败
			return { false };
		}
	}

	virtual std::vector<JobApplicationDTO> GetJobApplicationsByJobHunter(int id)  override
	{
		try {
			IPersistent::GetJobApplicationsByJobHunter(id);
		}
		catch (const std::exception& e) {
			// 处理异常，返回失败
			return { };
		}
	}

	virtual CompanyDTO GetCompanyById(int id)  override
	{
		try {
			IPersistent::GetCompanyById(id);
		}
		catch (const std::exception& e) {
			// 处理异常，返回失败
			return { };
		}
	}
	virtual StatusDTO UpdateCompanyInfo(int id, const Company& newInfo)  override
	{
		try {
			IPersistent::UpdateCompanyInfo(id, newInfo);
			return { true };
		}
		catch (const std::exception& e) {
			// 处理异常，返回失败
			return { false };
		}
	}

	int GetMatchingRate(const std::string& keyword, const std::string& name)
	{
		int x = 0;
		for (int i = 0; i < name.size() && x < keyword.size(); i++) {
			if (tolower(name[i]) == tolower(keyword[x]))  // 忽略大小写
				x++;
		}
		return x; // x 越大，说明匹配程度越高
	}

	std::vector<JobDTO> SearchJobs(const std::string& keyword, const std::string& location) override
	{
		try {
			std::vector<JobDTO> allJobs = GetAllJobs();

			std::vector<JobDTO> filtered;
			for (const auto& job : allJobs) {
				if (job.info.location == location) {
					filtered.push_back(job);
				}
			}

			std::vector<int> scores(filtered.size());
			for (size_t i = 0; i < filtered.size(); ++i) {
				scores[i] = GetMatchingRate(keyword, filtered[i].info.title);
			}

			for (size_t i = 0; i < filtered.size(); ++i) {
				for (size_t j = i + 1; j < filtered.size(); ++j) {
					if (scores[j] > scores[i]) {
						std::swap(filtered[i], filtered[j]);
						std::swap(scores[i], scores[j]);
					}
				}
			}
			return filtered;
		}
		catch (const std::exception& e) {
			std::cerr << "SearchJobs error: " << e.what() << std::endl;
			return {};
		}
	}


	virtual int CreateJob(const JobInfo& newJob) override
	{
		try {
			return IPersistent::CreateJob(newJob);
		}
		catch (const std::exception& e) {
			// 处理异常，返回失败
			return -1;
		}
	}

	virtual JobApplicationDTO ApplyForJob(int id) override
	{

		try {
			return IPersistent::ApplyForJob(id);
		}
		catch (const std::exception& e) {
			// 处理异常，返回失败
			return {};
		}
	}

	virtual std::vector<JobApplicationDTO> GetJobApplicationsByCompany(int id) override
	{
		try {
			return IPersistent::GetJobApplicationsByCompany(id);
		}
		catch (const std::exception& e) {
			// 处理异常，返回失败
			return {};
		}
	}


	StatusDTO UpdateJobApplicationStatus(int id, const string& status) override
	{
		try {
			IPersistent::UpdateJobApplicationStatus(id,status);
			return { true };
		}
		catch (const std::exception& e) {
			// 处理异常，返回失败
			return { false };
		}
	}


	virtual std::vector<Company> GetPendingCompaniesEdits() override
	{
		try {
			return IPersistent::GetPendingCompaniesEdits();
		}
		catch (const std::exception& e) {
			// 处理异常，返回失败
			return {};
		}
	}
	virtual std::vector<JobInfo> GetPendingJobsEdits() override
	{
		try {
			return IPersistent::GetPendingJobsEdits();
		}
		catch (const std::exception& e) {
			// 处理异常，返回失败
			return {};
		}
	}
	virtual StatusDTO CheckPending(const string& type, bool isApproved) override
	{
		try {
			IPersistent::CheckPending(type, isApproved);
			return { true };
		}
		catch (const std::exception& e) {
			// 处理异常，返回失败
			return { false };;
		}

	}
};