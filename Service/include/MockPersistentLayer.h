#pragma once

#include "DomainModels.h"
#include <memory>
#include <vector>
#include <map>
#include <optional>
#include <string>

using string = std::string;

class IMockPersistentLayer
{
public:
    virtual ~IMockPersistentLayer() = default;

    // 通用用户操作
    virtual std::unique_ptr<UserBase> GetUserById(const string &userId) = 0;
    virtual std::unique_ptr<UserBase> GetUserByEmail(const string &email) = 0;
    virtual bool UpdateUser(const UserBase &user) = 0;

    // 注册特定角色用户
    virtual bool AddJobHunter(const JobHunter &hunter) = 0;
    virtual bool AddCompany(const Company &company) = 0;
    virtual bool AddAdministrator(const Administrator &admin) = 0;

    // 领域实体获取和更新
    virtual std::unique_ptr<JobHunter> GetJobHunterProfile(const string &userId) = 0;
    virtual std::unique_ptr<Company> GetCompanyProfile(const string &userId) = 0;
    virtual bool UpdateJobHunter(const JobHunter &hunter) = 0;
    virtual bool UpdateCompany(const Company &company) = 0;

    // 岗位操作
    virtual std::unique_ptr<JobPosting> GetJobPosting(const string &jobId) = 0;
    virtual std::vector<JobPosting> GetAllJobPostings() = 0;
    virtual bool SaveJobPosting(const JobPosting &post) = 0;

    // 投递记录
    virtual std::unique_ptr<ApplicationRecord> GetApplicationRecord(const string &applicationId) = 0;
    virtual std::vector<ApplicationRecord> GetApplicationsByHunterId(const string &jobHunterId) = 0;
    virtual std::vector<ApplicationRecord> GetApplicationsByJobId(const string &jobId) = 0;
    virtual bool SaveApplicationRecord(const ApplicationRecord &record) = 0;
};
