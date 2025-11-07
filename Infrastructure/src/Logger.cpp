#include <Logger.h>
#include <iostream>
#include <cassert>

using std::cout;
using std::endl;
namespace {
	void SetConsoleColor(int color) {
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, color);
	}
}

void LogError(const std::string& text) {
	SetConsoleColor(FOREGROUND_RED | FOREGROUND_INTENSITY); // 红色加亮
	cout << text << endl;
	SetConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // 恢复默认白色
	exit(-1);
}
void LogError(const std::exception& ex) {
	LogError(ex.what());
}
void LogReq(const httplib::Request& req, bool withHeaders) {
	SetConsoleColor(FOREGROUND_GREEN);
	cout << "Request:" << endl;
	SetConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // 恢复默认白色
    std::ostringstream oss;

    oss << req.method << " " << req.path;
    if (!req.params.empty()) {
        oss << "?";
        bool first = true;
        for (auto& p : req.params) {
            if (!first) oss << "&";
            oss << p.first << "=" << p.second;
            first = false;
        }
    }
    oss << " HTTP/" << req.version << "\r\n";

    if (withHeaders) {
        for (auto& header : req.headers) {
            oss << header.first << ": " << header.second << "\r\n";
        }
    }

    oss << "\r\n";

    if (!req.body.empty()) {
        oss << req.body;
    }
    cout << oss.str() << endl;
    SetConsoleColor(FOREGROUND_GREEN);
    cout << "--Request END--" << endl << endl;
    SetConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // 恢复默认白色
}