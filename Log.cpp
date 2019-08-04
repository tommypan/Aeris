#include "Log.h"
#include <windows.h>
#include <windowsx.h>
#include <stdio.h>
#include <iostream>

Log::Log()
{
	AllocConsole();
	freopen("CONOUT$", "w+t", stdout);
	freopen("CONIN$", "r+t", stdin);
	
	/*static HANDLE consolehwnd;
	consolehwnd = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(consolehwnd, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE);*/
}

Log::~Log()
{
}

void Log::LogD(const std::string & str)
{
	Log::GetInstance();
	std::cout <<str.c_str()<< std::endl;
}

void Log::LogW(const std::string & str)
{

}

void Log::LogE(const std::string & str)
{

}

void Log::WriteToFile()
{

}