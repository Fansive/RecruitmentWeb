#pragma once

#include <string>
#include <vector>
#include <map>

// User Base Class
class UserBase
{
public:
    std::string userId;
    std::string passwordHash;

    std::string email;
    std::string role;    // JobHunter, Company, Administrator
    std::string name;    // 统一的名称
    std::string address; // 统一的地址信息

    virtual ~UserBase() = default;
};

// Job Hunter Class
class JobHunter : public UserBase
{
public:
    std::string gender;
    std::string phoneNumber;
    std::string degree;
    int graduationYear;
    std::string school;
    std::string major;
    std::string resumeUrl; // 简历文件URL
};

// Company Class
class Company : public UserBase
{
public:
    int id;
    std::string email, address, companyType, scale, contactInfo, description;
};

// Job Posting
struct JobInfo
{
    int id;
    std::string title, companyName, location, salary, description, degreeRequirement;
};



class Administrator : public UserBase
{
};