#include "ICompanyService.h"
#include "MockPersistentLayer.h"
#include <iostream>

// 模拟 Session ID 生成
static string GenerateSessionId()
{
    return "SID_" + std::to_string(std::time(nullptr));
}

class CompanyService : public ICompanyService
{
private:
    IMockPersistentLayer *_db;

public:
    CompanyService(IMockPersistentLayer *db) : _db(db) {}

    // IUserService Implementation (Company specific)
    RetType Signup(const string &id, const string &password, const string &role) override
    {
        if (role != "Company")
            return RetType::ServerError;

        if (_db->GetUserByEmail(id))
            return RetType::Signup_Existed;

        Company newCompany;
        newCompany.userId = "CO_" + std::to_string(std::time(nullptr));
        newCompany.email = id;
        newCompany.passwordHash = password;
        newCompany.role = role;
        newCompany.name = "New Company";

        if (_db->AddCompany(newCompany))
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
        if (userBase->role != "Company")
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

    // ICompanyService Implementation
    bool EditCompanyProfile(const string &companyId, const Company &company) override
    {
        auto existingProfile = _db->GetCompanyProfile(companyId);
        if (!existingProfile)
            return false;

        // 允许编辑所有公司信息字段
        Company updatedProfile = company;
        // 确保身份和安全字段不被修改 (保留原有值)
        updatedProfile.userId = existingProfile->userId;
        updatedProfile.email = existingProfile->email;
        updatedProfile.passwordHash = existingProfile->passwordHash;
        updatedProfile.role = existingProfile->role;

        return _db->UpdateCompany(updatedProfile);
    }

    std::unique_ptr<Company> GetCompanyProfile(const string &companyId) override
    {
        return _db->GetCompanyProfile(companyId);
    }

    bool PostNewJob(const string &companyId, const JobPosting &job) override
    {
        JobPosting newJob = job;
        newJob.jobId = "JOB_" + std::to_string(std::time(nullptr));
        newJob.companyId = companyId;

        // 业务逻辑：校验字段完整性
        if (newJob.title.empty() || newJob.description.empty())
            return false;

        return _db->SaveJobPosting(newJob);
    }

    bool EditJob(const string &jobId, const JobPosting &job) override
    {
        auto existingJob = _db->GetJobPosting(jobId);
        if (!existingJob)
            return false;

        JobPosting updatedJob = job;
        updatedJob.jobId = jobId;

        return _db->SaveJobPosting(updatedJob);
    }

    std::map<ApplicationRecord, JobHunter> ViewApplicationsForJob(const string &jobId) override
    {
        std::map<ApplicationRecord, JobHunter> result;
        auto records = _db->GetApplicationsByJobId(jobId);

        for (const auto &record : records)
        {
            auto hunter = _db->GetJobHunterProfile(record.jobHunterId);
            if (hunter)
            {
                result[record] = *hunter;
            }
        }
        return result;
    }

    bool MakeHiringDecision(const string &applicationId, const string &newStatus) override
    {
        auto record = _db->GetApplicationRecord(applicationId);
        if (!record)
            return false;

        // 业务逻辑：校验新状态的有效性 (e.g., "Accepted", "Rejected")
        if (newStatus != "Accepted" && newStatus != "Rejected")
            return false;

        record->status = newStatus;
        return _db->SaveApplicationRecord(*record);
    }
};
