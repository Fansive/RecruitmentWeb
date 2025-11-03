#ifndef I_JOB_HUNTER_SERVICE_H
#define I_JOB_HUNTER_SERVICE_H

#include "IUserService.h"
#include "DomainModels.h"

// 求职者业务逻辑服务接口
class IJobHunterService : public IUserService
{
public:
    virtual ~IJobHunterService() = default;

    // 编辑和更新个人信息（包含所有字段，如姓名、电话、学历、简历URL等）
    virtual bool editProfile(const std::string &userId, const JobHunter &profile) = 0;

    // 浏览岗位列表
    virtual std::vector<JobPosting> browseJobs(const std::string &keywords, const std::string &location) = 0;

    // 投递岗位
    virtual bool applyForJob(const std::string &jobHunterId, const std::string &jobId) = 0;

    // 查看已投递岗位信息
    virtual std::vector<ApplicationRecord> viewMyApplications(const std::string &jobHunterId) = 0;

    // 处理收到的Offer (同意或拒绝)
    virtual bool respondToOffer(const std::string &applicationId, bool accept) = 0;
};

#endif // I_JOB_HUNTER_SERVICE_H
