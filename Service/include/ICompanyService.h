#ifndef I_COMPANY_SERVICE_H
#define I_COMPANY_SERVICE_H

#include "IUserService.h"
#include "DomainModels.h"

// 招聘方业务逻辑服务接口 (操作主体为 Company)
class ICompanyService : public IUserService
{
public:
    virtual ~ICompanyService() = default;

    // 编辑或初始化公司信息
    virtual bool editCompanyProfile(const std::string &companyId, const Company &company) = 0;

    // 发布新岗位
    virtual bool postNewJob(const std::string &companyId, const JobPosting &job) = 0;

    // 编辑已发布的岗位
    virtual bool editJob(const std::string &jobId, const JobPosting &job) = 0;

    // 查看某个岗位的所有投递者简历列表 (ApplicationRecord 和 JobHunter 的组合信息)
    virtual std::map<ApplicationRecord, JobHunter> viewApplicationsForJob(const std::string &jobId) = 0;

    // 做出录用决定，newStatus 应该是 "Accepted" 或 "Rejected"
    virtual bool makeHiringDecision(const std::string &applicationId, const std::string &newStatus) = 0;
};

#endif // I_COMPANY_SERVICE_H
