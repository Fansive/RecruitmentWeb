#pragma once
#include <string>

class IFileLoader {
public:
	using string = std::string;
	virtual ~IFileLoader() = default;
	virtual std::string LoadText(const std::string& path) = 0;
};