#pragma once
#include <IFileLoader.h>

class SimpleFileLoader : public IFileLoader {
public:
	std::string LoadText(const std::string& path) override;
};