#pragma once
#include <string>
#include <iostream>

template<typename T>
inline void LogLine(T x) {
	std::cout << x << std::endl;
}
inline void LogLine() {
	std::cout << std::endl;
}
template<typename T>
inline void LogSpace(T x) {
	std::cout << x << " ";
}
void LogError(const std::string& text);
void LogError(const std::exception& ex);
