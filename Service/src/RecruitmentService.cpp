#include "IJobHunterService.h"
#include "IPersistentLayer.h"
#include <iostream>
#include <sstream>
#include <random>

// 模拟 Session ID 生成
static string GenerateSessionId()
{
    return "SID_" + std::to_string(std::time(nullptr));
}

class JobHunterService : public IJobHunterService
{
private:
    IPersistentLayer *_db;

public:
    JobHunterService(IPersistentLayer *db) : _db(db) {}

    // IUserService Implementation (Job Hunter specific)
    RetType Signup(const string &id, const string &password, const string &role) override
    {
        if (role != "JobHunter")
            return RetType::ServerError;

        if (_db->GetUserByEmail(id))
            return RetType::Signup_Existed;

        JobHunter newHunter;
        newHunter.userId = "JH_" + std::to_string(std::time(nullptr));
        newHunter.email = id;
        newHunter.passwordHash = password; // 简化：实际应使用哈希
        newHunter.role = role;
        newHunter.name = "New Hunter";

        if (_db->AddJobHunter(newHunter))
        {
            return RetType::Success;
        }
        return RetType::ServerError;
    }

    RetType Login(const string &id, const string &password, string &sessionId) override
    {
        auto userBase = _db->GetUserByEmail(id);
        if (!userBase || userBase->passwordHash != password)
        {
            return RetType::Login_Unmatch;
        }
        if (userBase->role != "JobHunter")
        {
            return RetType::Login_Unmatch; // 权限不匹配
        }

        sessionId = GenerateSessionId();
        // 实际：应将 sessionId 存入缓存或数据库
        return RetType::Success;
    }

    bool Logout(const string &sessionId) override
    {
        // 实际：应从缓存或数据库移除 sessionId
        return true;
    }

    std::unique_ptr<UserBase> GetUserBaseProfile(const string &userId) override
    {
        // 获取用户的基础信息，不包含专业 JobHunter 字段
        return _db->GetUserById(userId);
    }

    // IJobHunterService Implementation
    bool EditProfile(const string &userId, const JobHunter &profile) override
    {
        auto existingProfile = _db->GetJobHunterProfile(userId);
        if (!existingProfile)
            return false;

        JobHunter updatedProfile = profile;

        // 确保身份和安全字段不被修改 (保留原有值)
        updatedProfile.userId = existingProfile->userId;
        updatedProfile.email = existingProfile->email;
        updatedProfile.passwordHash = existingProfile->passwordHash;
        updatedProfile.role = existingProfile->role;

        return _db->UpdateJobHunter(updatedProfile);
    }

    std::unique_ptr<JobHunter> GetJobHunterProfile(const string &userId) override
    {
        return _db->GetJobHunterProfile(userId);
    }

    std::vector<JobPosting> BrowseAllJobs() override
    {
        return _db->GetAllJobPostings();
    }

    std::optional<string> ApplyToJob(const string &jobHunterId, const string &jobId) override
    {
        // 业务逻辑：检查是否重复投递
        auto applications = _db->GetApplicationsByHunterId(jobHunterId);
        for (const auto &app : applications)
        {
            if (app.jobId == jobId)
            {
                return std::nullopt; // 已投递
            }
        }

        ApplicationRecord newRecord;
        newRecord.applicationId = "APP_" + std::to_string(std::time(nullptr));
        newRecord.jobHunterId = jobHunterId;
        newRecord.jobId = jobId;
        newRecord.status = "Pending";

        if (_db->SaveApplicationRecord(newRecord))
        {
            return newRecord.applicationId;
        }
        return std::nullopt;
    }

    std::vector<ApplicationRecord> ViewMyApplications(const string &jobHunterId) override
    {
        return _db->GetApplicationsByHunterId(jobHunterId);
    }
};
