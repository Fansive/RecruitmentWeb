#include <SimpleFileLoader.h>
#include <fstream>
#include <sstream>
#include <iostream>

std::string SimpleFileLoader::LoadText(const std::string& path) {
	std::ifstream file(path);
	if (!file.is_open()) {
		std::cout << "´ò¿ªÊ§°Ü:" << path << std::endl;
		exit(0);
	}
	std::ostringstream ss;
	ss << file.rdbuf();
	return ss.str();
}
