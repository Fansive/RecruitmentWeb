#pragma once
#include <IPersistent.h>

class MySQLPersistent : public IPersistent{
public:
	using string = std::string;
	int CreateUser(const std::string& userId, const std::string& role) override;
	int GetUserIdByUserName(const std::string& userId) override;
	UserInfoDTO GetUserInfo(const int id) override;
	JobHunterDTO GetJobHunterInfo(int id) override;
	StatusDTO UpdateHunterInfo(int id, const JobHunter& newInfo) override;
	std::vector<JobApplicationDTO> GetJobApplicationsByJobHunter(int id) override;
	CompanyDTO GetCompanyById(int id) override;
	StatusDTO UpdateCompanyInfo(int id, const Company& newInfo) override;
	std::vector<JobDTO> GetAllJobs() override;
	int CreateJob(const JobInfo& newJob) override;
	JobApplicationDTO ApplyForJob(int id, int jobHunterId) override;
	std::vector<JobApplicationDTO> GetJobApplicationsByCompany(int id) override;
	StatusDTO UpdateJobApplicationStatus(int id, const string& status) override;
	std::vector<PendingReview> GetPendingCompaniesEdits() override;
	std::vector<PendingReview> GetPendingJobsEdits() override;
	StatusDTO CheckPending(const string& type, bool isApproved, int pendingReviewId) override;
};