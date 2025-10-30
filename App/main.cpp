#include <mysql/jdbc.h>
#include <iostream>
#include <memory>
#include <httplib.h>
#include <json.hpp>
#include <fstream>

namespace Config {
	constexpr auto Password_Path = "Others/local_password.txt";
	constexpr auto Server_URL = "http://localhost:8081";
}
std::string LoadPassword() {
	std::ifstream file(Config::Password_Path);
	if (!file.is_open()) {
		std::cout << Config::Password_Path << "打开失败" << std::endl;
		exit(0);
	}
	std::ostringstream ss;
	ss << file.rdbuf(); // read entire file into stringstream
	return ss.str();
}
 
int main() {
	try {
		std::string localPassword = LoadPassword();
		// 获取 MySQL 驱动实例
		sql::mysql::MySQL_Driver* driver = sql::mysql::get_mysql_driver_instance();

		// 建立连接
		std::unique_ptr<sql::Connection> con(
			driver->connect("tcp://127.0.0.1:3306", "root", localPassword)
		);

		// 选择数据库
		con->setSchema("testdb");

		// 创建 Statement
		std::unique_ptr<sql::Statement> stmt(con->createStatement());

		// 执行简单查询
		std::unique_ptr<sql::ResultSet> res(
			stmt->executeQuery("SELECT NOW() AS now_time")
		);

		// 输出查询结果
		while (res->next()) {
			std::cout << "Current time: " << res->getString("now_time") << std::endl;
		}

		std::cout << "MySQL Connector/C++ works correctly!" << std::endl;
	}
	catch (sql::SQLException& e) {
		std::cerr << "ERROR: SQLException: " << e.what() << std::endl;
		std::cerr << "MySQL error code: " << e.getErrorCode() << std::endl;
		std::cerr << "SQLState: " << e.getSQLState() << std::endl;
		return 1;
	}

	//测试httplib
	httplib::Server svr;

	svr.Get("/hello", [](const httplib::Request& req, httplib::Response& res) {
		res.set_content("Hello from httplib!", "text/plain");
		});

	// 在一个线程里运行服务器
	std::thread server_thread([&]() {
	std::cout << "Server is running at " << Config::Server_URL << std::endl;
	svr.listen("0.0.0.0", 8081);
	});

// 等服务器启动一点时间
	std::this_thread::sleep_for(std::chrono::milliseconds(500));

	// 使用 httplib 客户端访问刚启动的服务器
	httplib::Client cli(Config::Server_URL);
	if (auto res = cli.Get("/hello")) {
		std::cout << "Client received: " << res->body << std::endl;
	}
	else {
		std::cerr << "Client request failed!" << std::endl;
	}

	// 停止服务器
	svr.stop();
	server_thread.join();
	return 0;
}

