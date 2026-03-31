#pragma once
#include "Logger.h"

void LogColorPrint(LogColor color, const char* fmt, ...)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);

    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);

    SetConsoleTextAttribute(hConsole, LOG_DEFAULT);
}