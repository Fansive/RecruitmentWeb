#pragma once

#include "DomainModels.h"
#include <string>
#include <memory>

using string = std::string;

enum class RetType
{
    Success,
    Login_Unmatch,  // 账号或密码不匹配
    Signup_Existed, // 注册已存在的账号
    UserNotFound,
    ServerError
};

class IUserService
{
public:
    virtual ~IUserService() = default;

    // 核心认证服务
    virtual RetType Signup(const string &id, const string &password, const string &role) = 0;
    virtual RetType Login(const string &id, const string &password, string &sessionId) = 0;

    // 基础操作
    virtual bool Logout(const string &sessionId) = 0;
    // virtual bool ChangePassword(const string &userId, const string &oldPassword, const string &newPassword) = 0;
    virtual std::unique_ptr<UserBase> GetUserBaseProfile(const string &userId) = 0;
};
