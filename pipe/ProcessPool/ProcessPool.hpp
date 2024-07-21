#pragma once

#include <iostream>
#include <functional>
#include <vector>
#include <string>
#include <ctime>

using namespace std;

using task_t = function<void()>;
// typedef function<void()> task_t;

void Download()
{
    cout << "执行下载任务！" << "处理者：" << getpid() << endl;
}

void PrintLog()
{
    cout << "打印日志的任务" << "处理者：" << getpid() << endl;
}

void PushVideoStream()
{
    cout << "这是一个推送视频流的任务" << "处理者："<< getpid() << endl;
}

class Init
{
public:
    Init()
    {
        tasks.push_back(Download);
        tasks.push_back(PrintLog);
        tasks.push_back(PushVideoStream);

        // 产生随机数
        srand(time(nullptr) ^ getpid()); // 种一颗随机数种子
    }

    bool CheckSafe(int code)
    {
        if ((code >= 0) && (code < tasks.size()))
            return true;
        return false;
    }

    void RunTasks(int code)
    {
        tasks[code]();
    }

    int SelectTask()
    {
        return rand() % tasks.size();
    }

    string ToName(int code)
    {
        switch (code)
        {
        case download_code:
            return "Download";
        case printlog_code:
            return "PrintLog";
        case pushvideostream_code:
            return "PushVideoStream";
        default:
            return "UnKnow";
        }
    }

public:
    // 任务码
    const static int download_code = 0;
    const static int printlog_code = 1;
    const static int pushvideostream_code = 2;
    // 任务集合
    vector<task_t> tasks;
};

Init init; // 定义对象