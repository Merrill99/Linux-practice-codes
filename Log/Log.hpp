#pragma once

#include <iostream>
#include <string>
#include <cstdarg>
#include <cstdio>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

const std::string default_fliename = "Log.";
const std::string default_dir = "Log";

enum Fault
{
    Debug = 0,
    Info,
    Warning,
    Error,
    Fatal
};

// 输出方式，可以选择输出在屏幕或者文件里
enum output
{
    SCREEN = 10,
    ONEFILE,
    CLASSFILE
};

const int default_style = SCREEN;

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
    Log(int style = default_style, const std::string dirname = default_dir, 
        const std::string &filename = default_fliename)
        : _style(style), _dirname(dirname), _filename(filename)
    {
        mkdir(_dirname.c_str(), 0775);
    }

    // 将信息写入文件中
    void WriteToOneFile(const std::string &logname, const std::string &message)
    {
        umask(0);
        int fd = open(logname.c_str(), O_CREAT | O_WRONLY | O_APPEND, 06663);
        if (fd < 0)
            return;
        write(fd, message.c_str(), message.size());
        close(fd);
    }

    // 根据错误分类写入文件
    void WriteToClassFile(const std::string &level, const std::string &message)
    {
        std::string filename = default_dir;
        filename += "/";
        filename += default_fliename;
        filename += level;
        WriteToOneFile(filename, message);
    }

    // 选择打印方式写入
    void WriteLog(const std::string &level, const std::string &message)
    {
        switch (_style)
        {
        case SCREEN:
            std::cout << message;
            break;
        case ONEFILE:
            WriteToClassFile("all", message);
            break;
        case CLASSFILE:
            WriteToClassFile(level, message);
            break;
        default:
            break;
        }
    }

    std::string TransferTime()
    {
        // 将时间按格式化存入timebuffer中
        char timebuffer[128];
        time_t curtime = time(nullptr);
        struct tm* ct= localtime(&curtime);
        strftime(timebuffer, sizeof(timebuffer), "%Y-%m-%d %H:%M:%S", ct);
        // uint64_t curtime = time(nullptr);
        return timebuffer;
    }

    // [错误类型][时间][pid]"自己要输入的内容"
    void LogMessage(int level, const char *format, ...)
    {
        char leftbuffer[1024];
        std::string stringlevel = LevelToString(level); // 错误类型
        std::string curtime = TransferTime();             // 时间
        std::string pid = std::to_string(getpid());      // pid
        // 放入leftbuffer中
        snprintf(leftbuffer, sizeof(leftbuffer), "[%s][%s][%s]",
                 stringlevel.c_str(), curtime.c_str(), pid.c_str());

        char rightbuffer[1024]; // 需要打印的错误内容存放在这里
        va_list args;
        va_start(args, format);
        vsnprintf(rightbuffer, sizeof(rightbuffer), format, args);
        va_end(args); // 将args置为空

        std::string Info = leftbuffer;
        Info += rightbuffer;
        WriteLog(stringlevel, Info);
    }

    ~Log()
    {}

private:
    int _style;
    const std::string _dirname;
    const std::string _filename;
};

// 定义个全局变量方便使用
Log log;
