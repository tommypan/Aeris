#pragma once
#include <iostream>
#include <windows.h>
#include <windowsx.h>

class Log
{
public:
	Log();
	~Log();

public:
	static Log* GetInstance()
	{
		static Log log;
		return &log;
	}
	static void LogD(const std::string & str);
	static void LogW(const std::string & str);
	static void LogE(const std::string & str);
	static void WriteToFile();
private:
	static HANDLE _consolehwnd;
};