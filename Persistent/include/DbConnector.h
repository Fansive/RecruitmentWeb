#pragma once
#include <string>
#include <jdbc/cppconn/resultset.h>
#include <jdbc//cppconn/connection.h>
#include <SimpleFileLoader.h>

class DbConnector {//只负责和数据库连接,执行sql语句并返回原始的查询结果,但不对查询结果做任何处理
public:
	using string = std::string;
	using Entries = std::vector<std::map<string, string>>;
	static DbConnector Instance;
	bool Connect();
	bool Execute(const string& sql);
	Entries ExecuteQuery(const string& sql);
	int ExecuteUpdate(const string& sql); //返回值表示影响行数
private:
	sql::Driver* driver = nullptr;
	std::unique_ptr<sql::Connection> connection;
	void Disconnect();
	DbConnector() = default;
	~DbConnector() {
		Disconnect();
	}
	DbConnector(const DbConnector&) = delete;
	DbConnector operator=(const DbConnector&) = delete;
	DbConnector(DbConnector&&) = delete;
	DbConnector operator=(DbConnector&&) = delete;
};