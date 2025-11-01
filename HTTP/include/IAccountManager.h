#pragma once
#include <string>

class IAccountManager {
public:
	enum class RetType {
		Success, 
		Login_Unmatch, //账号或密码错误
		Signup_Existed, //注册已存在的账号
	};
	virtual ~IAccountManager() = default;
	//sessionId是输出参数,产生一个不会重复的字符串
	virtual RetType Login(const std::string& id, const std::string& password, std::string& sessionId) = 0;
	virtual RetType Signup(const std::string& id, const std::string& password, const std::string& role) = 0;
};