#pragma once

#include "IUserService.h"
#include "DomainModels.h"
#include <map>
#include <memory>

using string = std::string;

class ICompanyService : public IUserService
{
public:
    virtual ~ICompanyService() = default;

    // 公司信息管理
    virtual bool EditCompanyProfile(const string &companyId, const Company &company) = 0;
    virtual std::unique_ptr<Company> GetCompanyProfile(const string &companyId) = 0;

    // 岗位发布与管理
    virtual bool PostNewJob(const string &companyId, const JobPosting &job) = 0;
    virtual bool EditJob(const string &jobId, const JobPosting &job) = 0;

    // 申请管理
    virtual std::map<ApplicationRecord, JobHunter> ViewApplicationsForJob(const string &jobId) = 0;
    virtual bool MakeHiringDecision(const string &applicationId, const string &newStatus) = 0;
};
