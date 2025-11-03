#include "IJobHunterService.h"
#include "MockPersistentLayer.h"
#include <iostream>
#include <algorithm> // for std::find_if
#include <sstream>   // for UUID simulation

// 模拟 UUID 生成
std::string generateUUID()
{
    static int counter = 1000;
    std::stringstream ss;
    ss << "app-" << ++counter;
    return ss.str();
}

/**
 * 具体的求职者服务实现类
 */
class JobHunterServiceImpl : public IJobHunterService
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
    JobHunterServiceImpl(IMockPersistentLayer *dbLayer) : persistentLayer(dbLayer) {}

    // --- IUserService 继承方法实现 ---

    bool registerUser(const std::string &email, const std::string &password, const std::string &roleType) override
    {
        if (roleType != "JobHunter")
            return false;

        JobHunter newUser;
        newUser.userId = generateUUID();
        newUser.email = email;
        newUser.passwordHash = hashPassword(password);
        newUser.role = roleType;

        // 业务逻辑: 检查邮箱是否已存在
        // 模拟调用 Persistent Layer
        if (persistentLayer->getUserByEmail(email).has_value())
        {
            return false;
        }

        return persistentLayer->saveNewUser(newUser);
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
            // 返回一个堆上的对象副本，调用者负责释放
            if (user.role == "JobHunter")
            {
                return new JobHunter(persistentLayer->getJobHunterById(user.userId).value_or(JobHunter()));
            }
        }
        return nullptr;
    }

    bool changePassword(const std::string &userId, const std::string &newPassword) override
    {
        // 业务逻辑: 校验新密码复杂度（此处跳过）
        std::string newHash = hashPassword(newPassword);
        return persistentLayer->updatePassword(userId, newHash);
    }

    JobHunter *getJobHunterProfile(const std::string &userId) override
    {
        auto profileOpt = persistentLayer->getJobHunterById(userId);
        if (!profileOpt.has_value())
            return nullptr;

        // 返回一个堆上的对象副本
        return new JobHunter(profileOpt.value());
    }

    Company *getCompanyProfile(const std::string &userId) override
    {
        // 求职者服务不处理获取公司信息，但必须实现
        return nullptr;
    }

    // --- IJobHunterService 核心方法实现 ---

    bool editProfile(const std::string &userId, const JobHunter &profile) override
    {
        // 业务逻辑: 校验用户ID与Profile ID一致性
        if (userId != profile.userId)
            return false;

        // 模拟调用 Persistent Layer 更新 Profile
        return persistentLayer->updateJobHunterProfile(profile);
    }

    std::vector<JobPosting> browseJobs(const std::string &keywords, const std::string &location) override
    {
        // 业务逻辑: 可以加入过滤/排序逻辑（此处跳过）
        return persistentLayer->findJobs(keywords, location);
    }

    bool applyForJob(const std::string &jobHunterId, const std::string &jobId) override
    {
        // 业务逻辑: 1. 检查岗位是否存在; 2. 检查用户是否已投递过
        // 假设检查通过

        ApplicationRecord record;
        record.applicationId = generateUUID();
        record.jobHunterId = jobHunterId;
        record.jobId = jobId;
        record.status = "Pending";

        return persistentLayer->saveApplication(record);
    }

    std::vector<ApplicationRecord> viewMyApplications(const std::string &jobHunterId) override
    {
        return persistentLayer->findApplicationsByJobHunter(jobHunterId);
    }

    bool respondToOffer(const std::string &applicationId, bool accept) override
    {
        std::string newStatus = accept ? "Accepted" : "Rejected";
        // 业务逻辑: 校验ApplicationID状态是否为 Reviewed
        // 假设校验通过
        return persistentLayer->updateApplicationStatus(applicationId, newStatus);
    }
};

// Mock Persistent Layer 的简单实现（用于编译，Service层不关心其内部）
bool PersistentLayerImpl::saveNewUser(const UserBase &user) { return true; }
std::optional<UserBase> PersistentLayerImpl::getUserByEmail(const std::string &email) { return std::nullopt; }
bool PersistentLayerImpl::updatePassword(const std::string &userId, const std::string &newHash) { return true; }
std::optional<JobHunter> PersistentLayerImpl::getJobHunterById(const std::string &userId) { return JobHunter{}; }
bool PersistentLayerImpl::updateJobHunterProfile(const JobHunter &profile) { return true; }
std::optional<Company> PersistentLayerImpl::getCompanyById(const std::string &userId) { return Company{}; }
bool PersistentLayerImpl::updateCompanyProfile(const Company &profile) { return true; }
std::vector<JobPosting> PersistentLayerImpl::findJobs(const std::string &keywords, const std::string &location) { return {}; }
bool PersistentLayerImpl::saveJobPosting(const JobPosting &post) { return true; }
bool PersistentLayerImpl::updateJobPosting(const JobPosting &post) { return true; }
bool PersistentLayerImpl::saveApplication(const ApplicationRecord &record) { return true; }
std::vector<ApplicationRecord> PersistentLayerImpl::findApplicationsByJobHunter(const std::string &jobHunterId) { return {}; }
std::map<ApplicationRecord, JobHunter> PersistentLayerImpl::findApplicationsByJobId(const std::string &jobId) { return {}; }
bool PersistentLayerImpl::updateApplicationStatus(const std::string &applicationId, const std::string &newStatus) { return true; }
