#pragma once
#include <IFileLoader.h>

struct Cfg {
	std::string host, userName, password, database;
};
class SimpleFileLoader : public IFileLoader {
public:
	std::string LoadText(const std::string& path) override;
	Cfg LoadCfg(const std::string& path);
};
extern SimpleFileLoader g_FileLoader;