#pragma once
#include <string>
#include <vector>
#include <DomainModels.h>
#include <DTO.h>

// 持久层接口：定义数据存储/查询的抽象操作
class IPersistent {
public:
    using string = std::string;
    virtual ~IPersistent() = default;

    // -------------------------- 会话相关 --------------------------
    virtual bool existsSession(const string& sessionId) = 0;
    virtual void saveSession(const string& sessionId, const string& userId, const string& role) = 0;
    virtual UserInfoDTO getSessionUser(const string& sessionId) = 0;

    // -------------------------- 用户注册/登录 --------------------------
    virtual bool existsUser(const string& userId) = 0;
    virtual SignupDTO saveUser(const string& userId, const string& passwordHash, const string& role) = 0;
    virtual bool verifyUserPassword(const string& userId, const string& passwordHash) = 0;
    virtual string getUserRole(const string& userId) = 0;

    // -------------------------- 求职者信息 --------------------------
    virtual JobHunterDTO getJobHunterById(int id) = 0;
    virtual JobHunterDTO getJobHunterByUserId(const string& userId) = 0;
    virtual StatusDTO updateJobHunter(int id, const JobHunter& newInfo) = 0;

    // -------------------------- 公司信息 --------------------------
    virtual CompanyDTO getCompanyById(int id) = 0;
    virtual CompanyDTO getCompanyByUserId(const string& userId) = 0;
    virtual StatusDTO updateCompany(int id, const Company& newInfo) = 0;

    // -------------------------- 职位相关 --------------------------
    virtual std::vector<JobDTO> searchJobs(const string& keyword, const string& location) = 0;
    virtual int saveJob(const JobDTO& jobDTO) = 0;
    virtual JobInfo getJobById(int jobId) = 0;

    // -------------------------- 职位申请相关 --------------------------
    virtual JobApplicationDTO saveJobApplication(const JobApplicationDTO& application) = 0;
    virtual std::vector<JobApplicationDTO> getApplicationsByHunterId(int hunterId) = 0;
    virtual std::vector<JobApplicationDTO> getApplicationsByCompanyId(int companyId) = 0;
    virtual StatusDTO updateApplicationStatus(int applicationId, const string& newStatus) = 0;

    // -------------------------- 管理员审核相关 --------------------------
    virtual std::vector<Company> getPendingCompanyEdits() = 0;
    virtual std::vector<JobInfo> getPendingJobEdits() = 0;
    virtual StatusDTO approvePendingEdit(const string& type, int targetId, bool isApproved) = 0;
};