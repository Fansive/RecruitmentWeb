#pragma once
#include <string>
#include <vector>
#include <random>
#include <algorithm>
#include "IService.h"
#include "IPersistent.h"
#include "DomainModels.h"
#include "DTO.h"

// 服务层实现：处理业务逻辑，依赖持久层接口
class AppService : public IService {
public:
    using string = std::string;

    // 构造函数注入持久层依赖（依赖倒置）
    explicit AppService(IPersistent* persistent) : m_persistent(persistent) {}
    ~AppService() override = default;

    // -------------------------- 会话相关 --------------------------
    bool IsSessionExists(const string& sessionId) override {
        return m_persistent->existsSession(sessionId);
    }

    StatusDTO Login(const string& userId, const string& password, const string& role, string& sessionId) override {
        StatusDTO result{ false };

        // 1. 验证用户存在性
        if (!m_persistent->existsUser(userId)) {
            return result;
        }

        // 2. 验证密码（假设密码已哈希，这里直接传递密码字符串，实际应在服务层哈希后再传递）
        string passwordHash = password; // 实际项目中需替换为哈希算法（如SHA256）
        if (!m_persistent->verifyUserPassword(userId, passwordHash)) {
            return result;
        }

        // 3. 验证角色匹配
        if (m_persistent->getUserRole(userId) != role) {
            return result;
        }

        // 4. 生成唯一会话ID（简化实现：随机字符串）
        sessionId = generateUniqueSessionId();

        // 5. 保存会话到持久层
        m_persistent->saveSession(sessionId, userId, role);

        result.success = true;
        return result;
    }

    SignupDTO Signup(const string& userId, const string& password, const string& role, bool& success) override {
        SignupDTO result{ "", userId };
        success = false;

        // 1. 检查用户是否已存在
        if (m_persistent->existsUser(userId)) {
            return result;
        }

        // 2. 密码哈希（实际项目中需使用安全哈希算法）
        string passwordHash = password;

        // 3. 保存用户到持久层
        result = m_persistent->saveUser(userId, passwordHash, role);
        success = !result.id.empty();

        return result;
    }

    UserInfoDTO GetMe(const string& sessionId) override {
        // 1. 验证会话存在
        if (!m_persistent->existsSession(sessionId)) {
            return UserInfoDTO{ "", "", "", "" };
        }

        // 2. 从持久层获取会话对应的用户信息
        return m_persistent->getSessionUser(sessionId);
    }

    // -------------------------- 求职者相关 --------------------------
    JobHunterDTO GetJobHunterInfo(int id) override {
        return m_persistent->getJobHunterById(id);
    }

    StatusDTO UpdateHunterInfo(int id, const JobHunter& newInfo) override {
        // 业务逻辑：可添加字段验证（如手机号格式、毕业年份合理性等）
        return m_persistent->updateJobHunter(id, newInfo);
    }

    std::vector<JobApplicationDTO> GetJobApplicationsByJobHunter(int id) override {
        return m_persistent->getApplicationsByHunterId(id);
    }

    // -------------------------- 公司相关 --------------------------
    CompanyDTO GetCompanyById(int id) override {
        return m_persistent->getCompanyById(id);
    }

    StatusDTO UpdateCompanyInfo(int id, const Company& newInfo) override {
        // 业务逻辑：可添加公司信息验证（如联系方式格式、规模合法性等）
        return m_persistent->updateCompany(id, newInfo);
    }

    // -------------------------- 职位相关 --------------------------
    std::vector<JobDTO> SearchJobs(const string& keyword, const string& location) override {
        // 业务逻辑：关键词模糊匹配（持久层实现具体查询逻辑）
        return m_persistent->searchJobs(keyword, location);
    }

    int CreateJob(const JobInfo& newJob) override {
        // 1. 从会话中获取当前登录公司的ID（需优化：通过SessionId获取公司信息）
        // 这里简化：假设当前会话已验证，通过UserInfoDTO获取公司ID
        UserInfoDTO currentUser = GetMe(m_currentSessionId); // 实际需传入SessionId，这里简化为成员变量
        CompanyDTO company = m_persistent->getCompanyByUserId(currentUser.userId);

        // 2. 构造JobDTO
        JobDTO jobDTO{
            newJob,
            company.companyInfo.id,
            getCurrentTime() // 生成创建时间（简化实现）
        };

        // 3. 保存职位到持久层
        return m_persistent->saveJob(jobDTO);
    }

    // -------------------------- 职位申请相关 --------------------------
    JobApplicationDTO ApplyForJob(int jobId) override {
        // 1. 验证职位存在
        JobInfo job = m_persistent->getJobById(jobId);
        if (job.id == 0) {
            return JobApplicationDTO{ "", "", "", "invalid_job", JobInfo{} };
        }

        // 2. 获取当前登录求职者信息
        UserInfoDTO currentUser = GetMe(m_currentSessionId);
        JobHunterDTO hunter = m_persistent->getJobHunterByUserId(currentUser.userId);

        // 3. 构造申请DTO
        JobApplicationDTO application{
            generateUniqueId(), // 申请ID
            std::to_string(jobId),
            getCurrentTime(),
            "pending", // 初始状态：待审核
            job
        };

        // 4. 保存申请到持久层
        return m_persistent->saveJobApplication(application);
    }

    std::vector<JobApplicationDTO> GetJobApplicationsByCompany(int id) override {
        return m_persistent->getApplicationsByCompanyId(id);
    }

    StatusDTO UpdateJobApplicationStatus(int id) override {
        // 简化：默认更新为"approved"，实际可扩展为传入状态参数
        return m_persistent->updateApplicationStatus(id, "approved");
    }

    // -------------------------- 管理员相关 --------------------------
    std::vector<Company> GetPendingCompaniesEdits() override {
        return m_persistent->getPendingCompanyEdits();
    }

    std::vector<JobInfo> GetPendingJobsEdits() override {
        return m_persistent->getPendingJobEdits();
    }

    StatusDTO CheckPending(const string& type, bool isApproved) override {
        // 简化：假设type格式为"company_123"或"job_456"，解析目标ID
        size_t sep = type.find('_');
        if (sep == string::npos) {
            return StatusDTO{ false };
        }

        string targetType = type.substr(0, sep);
        int targetId = std::stoi(type.substr(sep + 1));

        return m_persistent->approvePendingEdit(targetType, targetId, isApproved);
    }

private:
    IPersistent* m_persistent; // 依赖持久层接口（解耦具体实现）
    string m_currentSessionId; // 简化：当前会话ID（实际应从请求上下文获取）

    // 生成唯一会话ID（简化实现：随机32位字符串）
    string generateUniqueSessionId() {
        static const char chars[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<> dis(0, sizeof(chars) - 2);

        string sessionId;
        for (int i = 0; i < 32; ++i) {
            sessionId += chars[dis(gen)];
        }
        return sessionId;
    }

    // 生成唯一ID（用于申请ID、用户ID等）
    string generateUniqueId() {
        // 实际项目中可使用UUID或数据库自增ID
        static int counter = 0;
        return "ID_" + std::to_string(++counter) + "_" + std::to_string(time(nullptr));
    }

    // 获取当前时间（格式：YYYY-MM-DD HH:MM:SS）
    string getCurrentTime() {
        time_t now = time(nullptr);
        tm tm = *localtime(&now);
        char buf[64];
        strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &tm);
        return buf;
    }
};