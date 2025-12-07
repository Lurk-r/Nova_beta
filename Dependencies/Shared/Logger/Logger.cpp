#include "Logger.hpp"
#include <windows.h>
#include <cstdio>

void Logger::SetColor(Type color)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), static_cast<WORD>(color));
}

void Logger::Print(const char* prefix, const std::string& message)
{
    printf("[%s] %s\n", prefix, message.c_str());
}