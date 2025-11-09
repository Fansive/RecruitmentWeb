#include <SimpleFileLoader.h>
#include <fstream>
#include <sstream>
#include <iostream>

SimpleFileLoader g_FileLoader;
std::string SimpleFileLoader::LoadText(const std::string& path) {
	std::ifstream file(path);
	if (!file.is_open()) {
		exit(0);
	}
	std::ostringstream ss;
	ss << file.rdbuf();
	return ss.str();
}

Cfg SimpleFileLoader::LoadCfg(const std::string& path)
{
	std::string text = LoadText(path);
	size_t sep1 = text.find('\n');
	size_t sep2 = text.find('\n', sep1 + 1);
	size_t sep3 = text.find('\n', sep2 + 1);
	return Cfg{ text.substr(0, sep1), text.substr(sep1 + 1, sep2 - sep1 - 1),text.substr(sep2 + 1, sep3 - sep2 - 1),text.substr(sep3 + 1, text.size() - sep3 - 1) };
}
