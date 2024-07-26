#pragma once

#include <iostream>
#include <string>
#include <cstdarg>
#include <cstdio>

enum Fault
{
    Debug = 0,
    Info,
    Warning,
    Error,
    Fatal
};

std::string LevelToString(int level)
{
    switch (level)
    {
    case Debug:
        return "Debug";
        break;
    case Info:
        return "Info";
        break;
    case Warning:
        return "Warning";
        break;
    case Error:
        return "Error";
        break;
    case Fatal:
        return "Fatal";
        break;
    default:
        return "Unknow";
        break;
    }
}

class Log
{
public:
    Log()
    {}

    void LogMessage(int level, const char* format, ...)
    {
        char content[1024]; // 需要打印的错误内容存放在这里
        va_list args;
        va_start(args, format);
        vsnprintf(content, sizeof(content), format, args);
        va_end(args); // 将args置为空
        time_t curtime = time(nullptr);   
        // uint64_t curtime = time(nullptr);
        printf("[%s][%s]%s\n", LevelToString(level).c_str(), std::to_string(curtime).c_str(), content);
    }
    
    ~Log()
    {}
private:
};