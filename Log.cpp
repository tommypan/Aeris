#include "Log.h"
#include <stdio.h>
#include <iostream>

HANDLE Log::_consolehwnd;

Log::Log()
{
	AllocConsole();
	freopen("CONOUT$", "w+t", stdout);
	freopen("CONIN$", "r+t", stdin);
	Log::_consolehwnd = GetStdHandle(STD_OUTPUT_HANDLE);
}

Log::~Log()
{
}

void Log::LogD(const std::string & str)
{
	Log::GetInstance();

	SetConsoleTextAttribute(_consolehwnd, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	std::cout <<str.c_str()<< std::endl;
}

void Log::LogW(const std::string & str)
{
	Log::GetInstance();
	SetConsoleTextAttribute(_consolehwnd, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
	std::cout << str.c_str() << std::endl;
}

void Log::LogE(const std::string & str)
{
	Log::GetInstance();
	SetConsoleTextAttribute(_consolehwnd, FOREGROUND_RED | FOREGROUND_INTENSITY);
	std::cout << str.c_str() << std::endl;
}

void Log::WriteToFile()
{

}