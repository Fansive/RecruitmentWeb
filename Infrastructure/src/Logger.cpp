#include <Windows.h>
#include <Logger.h>
#include <iostream>
#include <cassert>

namespace {
	void SetConsoleColor(int color) {
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, color);
	}
}

void LogError(const std::string& text) {
	SetConsoleColor(FOREGROUND_RED | FOREGROUND_INTENSITY); // 红色加亮
	std::cout << text << std::endl;
	SetConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // 恢复默认白色
	exit(-1);
}
void LogError(const std::exception& ex) {
	LogError(ex.what());
}