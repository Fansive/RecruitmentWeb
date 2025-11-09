#pragma once
#include <string>
#include <vector>
#include <DTO.h>
#include <DomainModels.h>

class IService {
public:
	using string = std::string;
	virtual ~IService() = default;
	//sessionId是输出参数,产生一个不会重复的字符串
	virtual bool IsSessionExists(const string& sessionId) = 0;
	virtual StatusDTO Login(const string& userId, const string& password, const string &role, string& sessionId) = 0;
	virtual SignupDTO Signup(const string& userId, const string& password, const string& role, bool &success) = 0;
	virtual UserInfoDTO GetMe(const string& sessionId) = 0;

	virtual JobHunterDTO GetJobHunterInfo(int id) = 0;
	virtual StatusDTO UpdateHunterInfo(int id, const JobHunter& newInfo) = 0;
	//根据求职者id,查询他的投递列表
	virtual std::vector<JobApplicationDTO> GetJobApplicationsByJobHunter(int id) = 0;
	virtual CompanyDTO GetCompanyById(int id) = 0;
	virtual StatusDTO UpdateCompanyInfo(int id, const Company& newInfo) = 0;
	virtual std::vector<JobDTO> SearchJobs(const string& keyword, const string& location) = 0;
	//该job的companyId通过sessionId获取,该函数返回值是该job的id
	virtual int CreateJob(const JobInfo& newJob) = 0;
	//该id为job的id
	virtual JobApplicationDTO ApplyForJob(int id, int jobHunterId) = 0;
	//根据公司id,查询他的岗位申请列表
	virtual std::vector<JobApplicationDTO> GetJobApplicationsByCompany(int id) = 0;
	//公司同意/拒绝职位申请后,更新招聘进度,该id为jobApplication的id
	virtual StatusDTO UpdateJobApplicationStatus(int id, const string& status) = 0;
	//管理员获取公司修改信息的申请列表
	virtual std::vector<PendingReview> GetPendingCompaniesEdits() = 0;
	virtual std::vector<PendingReview> GetPendingJobsEdits() = 0;
	//管理员审核请求
	virtual StatusDTO CheckPending(const string& type, bool isApproved, int pendingReviewId) = 0;

};