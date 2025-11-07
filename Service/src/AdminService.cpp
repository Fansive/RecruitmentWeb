#include "IAdminService.h"
#include "IPersistentLayer.h"
#include <iostream>

// 模拟 Session ID 生成
static string GenerateSessionId()
{
    return "SID_" + std::to_string(std::time(nullptr));
}

class AdminService : public IAdminService
{
private:
    IPersistentLayer *_db;

public:
    AdminService(IPersistentLayer *db) : _db(db) {}

    // IUserService Implementation (Admin specific)
    RetType Signup(const string &id, const string &password, const string &role) override
    {
        // 业务逻辑：确保只允许创建第一个管理员账户 (First-Time Setup)
        if (role != "Administrator")
        {
            return RetType::ServerError;
        }

        const string Admin_ID = "Admin001";
        if (_db->GetUserById(Admin_ID))
        {
            return RetType::Signup_Existed;
        }

        // 检查邮箱是否已被使用
        if (_db->GetUserByEmail(id))
        {
            return RetType::Signup_Existed;
        }

        // 仅在没有管理员存在且邮箱未被使用时允许注册
        Administrator newAdmin;
        newAdmin.userId = Admin_ID; // 首次管理员注册使用特殊固定 ID
        newAdmin.email = id;
        newAdmin.passwordHash = password;
        newAdmin.role = role;
        newAdmin.name = "System Administrator";

        if (_db->AddAdministrator(newAdmin))
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
        if (userBase->role != "Administrator")
        {
            return RetType::Login_Unmatch;
        }

        sessionId = GenerateSessionId();
        return RetType::Success;
    }

    bool Logout(const string &sessionId) override
    {
        return true;
    }

    std::unique_ptr<UserBase> GetUserBaseProfile(const string &userId) override
    {
        return _db->GetUserById(userId);
    }

    // IAdminService Implementation
    bool ReviewJobPosting(const string &jobId, bool approve) override
    {
        // 业务逻辑：更新 JobPosting 状态
        auto job = _db->GetJobPosting(jobId);
        if (!job)
            return false;

        // 实际：job->status = approve ? "Approved" : "Rejected";
        return _db->SaveJobPosting(*job);
    }
};
