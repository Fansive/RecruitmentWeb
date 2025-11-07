#pragma once
#include <string>
#include <DomainModels.h>

struct StatusDTO {
	bool success;
};
struct SignupDTO {
	std::string id, userId;
};
struct UserInfoDTO {
	std::string id, userId, role, email;
};
struct JobHunterDTO {
	JobHunter info;
	std::string id, userId, createdAt;
};
struct JobApplicationDTO {
    std::string id, jobId, appliedAt, status;
    JobInfo jobInfo;
};
struct CompanyDTO {
	std::string id, userId, createdAt;
	Company companyInfo;
};
struct JobDTO {
	JobInfo info;
	int companyId;
	std::string createdAt;
};
