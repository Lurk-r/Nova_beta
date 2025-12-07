#pragma once
#include <format>
#include <string>

enum class Type
{
    Reset = 7,
    Info = 3,
    Warning = 14,
    Error = 12,
    Success = 10
};

class Logger
{
public:
    template<typename... Args>
    static void Info(std::format_string<Args...> fmt, Args&&... args)
    {
        Log(Type::Info, "INFO", fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    static void Warning(std::format_string<Args...> fmt, Args&&... args)
    {
        Log(Type::Warning, "WARNING", fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    static void Error(std::format_string<Args...> fmt, Args&&... args)
    {
        Log(Type::Error, "ERROR", fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    static void Success(std::format_string<Args...> fmt, Args&&... args)
    {
        Log(Type::Success, "SUCCESS", fmt, std::forward<Args>(args)...);
    }

private:
    static void SetColor(Type color);
    static void Print(const char* prefix, const std::string& message);

    template<typename... Args>
    static void Log(Type color, const char* prefix, std::format_string<Args...> fmt, Args&&... args)
    {
        SetColor(color);
        Print(prefix, std::format(fmt, std::forward<Args>(args)...));
        SetColor(Type::Reset);
    }
};