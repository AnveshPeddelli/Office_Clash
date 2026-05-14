#pragma once
#include <Windows.h>
#include <stdio.h>
#include <cstdarg>

enum LogColor
{
	LOG_DEFAULT = 7,
	LOG_RED = 12,
	LOG_GREEN = 10,
	LOG_YELLOW = 14,
	LOG_BLUE = 9
};

void LogColorPrint(LogColor color, const char* fmt, ...);

#define LOG_I(...) LogColorPrint(LOG_GREEN, __VA_ARGS__)
#define LOG_W(...) LogColorPrint(LOG_YELLOW, __VA_ARGS__)
#define LOG_E(...) LogColorPrint(LOG_RED, __VA_ARGS__)
#define LOG_D(...) LogColorPrint(LOG_BLUE, __VA_ARGS__)
#define LOG(...) LogColorPrint(LOG_DEFAULT, __VA_ARGS__)
