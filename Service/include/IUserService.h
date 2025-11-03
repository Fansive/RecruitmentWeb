#ifndef I_USER_SERVICE_H
#define I_USER_SERVICE_H

#include "DomainModels.h"

class IUserService
{
public:
    virtual ~IUserService() = default;

    // 注册新用户，roleType应为 "JobHunter" 或 "Company"
    virtual bool registerUser(const std::string &email, const std::string &password, const std::string &roleType) = 0;

    // 用户登录
    virtual UserBase *login(const std::string &email, const std::string &password) = 0;

    // 更改密码
    virtual bool changePassword(const std::string &userId, const std::string &newPassword) = 0;

    // 获取求职者Profile
    virtual JobHunter *getJobHunterProfile(const std::string &userId) = 0;

    // 获取公司Profile
    virtual Company *getCompanyProfile(const std::string &userId) = 0;
};

#endif // I_USER_SERVICE_H
