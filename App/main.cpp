#include <Server.h>

//namespace Config {
//	constexpr auto Password_Path = "Others/local_password.txt";
//	constexpr auto Server_URL = "http://localhost:8081";
//}
//std::string LoadPassword() {
//	std::ifstream file(Config::Password_Path);
//	if (!file.is_open()) {
//		std::cout << Config::Password_Path << "´ò¿ªÊ§°Ü" << std::endl;
//		exit(0);
//	}
//	std::ostringstream ss;
//	ss << file.rdbuf(); // read entire file into stringstream
//	return ss.str();
//}
 
int main() {

	std::cout << HttpHandler::GetURL();
	HttpHandler::Instance().RegisterRoutingAndRun();
	return 0;
}

