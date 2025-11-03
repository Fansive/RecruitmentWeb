#pragma once
#include <string>

class IService {
public:
	using string = std::string;
	enum class RetType {
		Success, 
		Login_Unmatch, //账号或密码错误
		Signup_Existed, //注册已存在的账号
		Session_Existed, Session_Nonexisted
	};
	virtual ~IService() = default;
	//sessionId是输出参数,产生一个不会重复的字符串
	virtual RetType Login(const string& id, const string& password, string& sessionId) = 0;
	virtual RetType Signup(const string& id, const string& password, const string& role) = 0;
	virtual RetType IsSessionExists(const string& sessionId) = 0;
};