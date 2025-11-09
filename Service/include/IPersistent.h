#pragma once
#include <string>
#include "DTO.h"

class IPersistent {
public:
    virtual ~IPersistent() = default;


    //  创建用户，返回数据库生成的主键 id
    virtual int CreateUser(const std::string& userId, const std::string& role) = 0;

    //  通过 userId 查找主键 id
    virtual int GetUserIdByUserName(const std::string& userId) = 0;

    // 通过id获取用户完整DTO信息
    virtual UserInfoDTO GetUserInfo(const int id) = 0;

    //JobHnter相关
    virtual JobHunterDTO GetJobHunterInfo(int id) = 0;
    virtual StatusDTO UpdateHunterInfo(int id, const JobHunter& newInfo) = 0;
    virtual std::vector<JobApplicationDTO> GetJobApplicationsByJobHunter(int id) = 0;

    //Company相关
    virtual CompanyDTO GetCompanyById(int id) = 0;
    virtual StatusDTO UpdateCompanyInfo(int id, const Company& newInfo) = 0;

    virtual std::vector<JobDTO> GetAllJobs() = 0;

    //创建JobInfo，返回该job的id
    virtual int CreateJob(const JobInfo& newJob) = 0;


    //---JobApplication相关
    // 
    //该id为job的id
    virtual JobApplicationDTO ApplyForJob(int id, int jobHunterId) = 0;
    //根据公司id,查询他的岗位申请列表
    virtual std::vector<JobApplicationDTO> GetJobApplicationsByCompany(int id) = 0;

    //公司同意/拒绝职位申请后,更新招聘进度,该id为jobApplication的id
    virtual StatusDTO UpdateJobApplicationStatus(int id, const std::string& status) = 0;

    virtual std::vector<PendingReview> GetPendingCompaniesEdits() = 0;
    virtual std::vector<PendingReview> GetPendingJobsEdits() = 0;

    virtual StatusDTO CheckPending(const std::string& type, bool isApproved, int pendingReviewId) = 0;

};