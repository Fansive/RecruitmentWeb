#ifndef DOMAIN_MODELS_H
#define DOMAIN_MODELS_H

#include <string>
#include <vector>
#include <map>

// User Base Class
class UserBase
{
public:
    std::string userId; // 用户ID
    std::string email;  // 邮箱
    std::string passwordHash;
    std::string role; // 角色标识

    virtual ~UserBase() = default;
};

// Job Hunter Profile
class JobHunter : public UserBase
{
public:
    std::string name;
    std::string gender;
    std::string phoneNumber;
    std::string currentAddress;
    std::string degree;
    int graduationYear;
    std::string school;
    std::string major;
    std::string resumeUrl; // 简历文件URL
};

// Company Profile (acts as Recruiter)
class Company : public UserBase
{
public:
    std::string name;
    std::string type;
    std::string scale;
    std::string address;
    std::string contactInfo;
    std::string description;
};

// Job Posting
struct JobPosting
{
    std::string jobId;
    std::string companyId;
    std::string title;
    std::string description;
    std::string requirements;
    std::string location;
};

// Application Record (M:N Relationship)
struct ApplicationRecord
{
    std::string applicationId;
    std::string jobHunterId;
    std::string jobId;

    // Status: Pending, Reviewed, Accepted, Rejected
    std::string status;
};

#endif // DOMAIN_MODELS_H
