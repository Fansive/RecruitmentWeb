#pragma once

#include "IUserService.h"
#include "DomainModels.h"
#include <vector>
#include <memory>
#include <optional>

using string = std::string;

class IJobHunterService : public IUserService
{
public:
    virtual ~IJobHunterService() = default;

    // 个人信息管理
    virtual bool EditProfile(const string &userId, const JobHunter &profile) = 0;
    virtual std::unique_ptr<JobHunter> GetJobHunterProfile(const string &userId) = 0;

    // 岗位浏览与投递
    virtual std::vector<JobPosting> BrowseAllJobs() = 0;
    virtual std::optional<string> ApplyToJob(const string &jobHunterId, const string &jobId) = 0;

    // 投递记录查询
    virtual std::vector<ApplicationRecord> ViewMyApplications(const string &jobHunterId) = 0;
};
