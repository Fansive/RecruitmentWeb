#include "ICompanyService.h"
#include "MockPersistentLayer.h"
#include <iostream>

// 模拟 UUID 生成（与 JobHunterService.cpp 中的函数保持一致）
std::string generateUUIDCompany()
{
    static int counter = 2000;
    std::stringstream ss;
    ss << "comp-" << ++counter;
    return ss.str();
}

/**
 * 具体的公司服务实现类
 */
class CompanyServiceImpl : public ICompanyService
{
private:
    IMockPersistentLayer *persistentLayer;

    // 模拟密码哈希函数
    std::string hashPassword(const std::string &password) const
    {
        return "hashed_" + password;
    }

public:
    // 通过构造函数进行依赖注入，保持低耦合
    CompanyServiceImpl(IMockPersistentLayer *dbLayer) : persistentLayer(dbLayer) {}

    // --- IUserService 继承方法实现 ---

    bool registerUser(const std::string &email, const std::string &password, const std::string &roleType) override
    {
        if (roleType != "Company")
            return false;

        Company newCompany;
        newCompany.userId = generateUUIDCompany();
        newCompany.email = email;
        newCompany.passwordHash = hashPassword(password);
        newCompany.role = roleType;

        // 业务逻辑: 检查邮箱是否已存在
        if (persistentLayer->getUserByEmail(email).has_value())
        {
            return false;
        }

        return persistentLayer->saveNewUser(newCompany);
    }

    UserBase *login(const std::string &email, const std::string &password) override
    {
        auto userOpt = persistentLayer->getUserByEmail(email);
        if (!userOpt.has_value())
            return nullptr;

        UserBase &user = userOpt.value();
        // 业务逻辑: 验证密码
        if (user.passwordHash == hashPassword(password))
        {
            // 返回一个堆上的对象副本
            if (user.role == "Company")
            {
                return new Company(persistentLayer->getCompanyById(user.userId).value_or(Company()));
            }
        }
        return nullptr;
    }

    bool changePassword(const std::string &userId, const std::string &newPassword) override
    {
        std::string newHash = hashPassword(newPassword);
        return persistentLayer->updatePassword(userId, newHash);
    }

    JobHunter *getJobHunterProfile(const std::string &userId) override
    {
        // 公司服务不处理获取求职者信息，但必须实现
        return nullptr;
    }

    Company *getCompanyProfile(const std::string &userId) override
    {
        auto profileOpt = persistentLayer->getCompanyById(userId);
        if (!profileOpt.has_value())
            return nullptr;

        // 返回一个堆上的对象副本
        return new Company(profileOpt.value());
    }

    // --- ICompanyService 核心方法实现 ---

    bool editCompanyProfile(const std::string &companyId, const Company &company) override
    {
        // 业务逻辑: 校验 Company ID 一致性
        if (companyId != company.userId)
            return false;

        return persistentLayer->updateCompanyProfile(company);
    }

    bool postNewJob(const std::string &companyId, const JobPosting &job) override
    {
        // 业务逻辑: 校验 Job Posting 中的 companyId 是否匹配
        if (companyId != job.companyId)
            return false;

        JobPosting newJob = job;
        newJob.jobId = generateUUIDCompany(); // 模拟生成 Job ID

        return persistentLayer->saveJobPosting(newJob);
    }

    bool editJob(const std::string &jobId, const JobPosting &job) override
    {
        // 业务逻辑: 校验岗位是否存在、权限是否属于该公司
        // 假设校验通过
        return persistentLayer->updateJobPosting(job);
    }

    std::map<ApplicationRecord, JobHunter> viewApplicationsForJob(const std::string &jobId) override
    {
        // 业务逻辑: Persistent Layer 返回 ApplicationRecord 和 JobHunter 信息的 Map
        return persistentLayer->findApplicationsByJobId(jobId);
    }

    bool makeHiringDecision(const std::string &applicationId, const std::string &newStatus) override
    {
        // 业务逻辑: 1. 校验 Application ID 存在性; 2. 校验 newStatus 合法性
        if (newStatus != "Accepted" && newStatus != "Rejected")
            return false;

        return persistentLayer->updateApplicationStatus(applicationId, newStatus);
    }
};
