#pragma once

#include <iostream>
#include <queue>
#include <vector>
#include <memory>
#include <pthread.h>
#include "Log.hpp"
#include "LockGuard.hpp"
#include "thread.hpp"
#include "Task.hpp"

const int defualt_num = 5; // 默认线程池的数量

class ThreadData
{
public:
    ThreadData(std::string& threadname)
        :_threadname(threadname)
    {}

    const std::string& GetName() const
    {
        return _threadname;
    }

    ~ThreadData()
    {}
private:
    std::string _threadname;
};

template <class T>
class PthreadPool
{
public:
    PthreadPool(int pthread_num = defualt_num)
        :_pthread_num(pthread_num)
    {
        pthread_mutex_init(&_mutex, nullptr);
        pthread_cond_init(&_cond, nullptr);
        // 创建线程
        for (int i = 0; i < _pthread_num; ++i)
        {
            std::string threadname = "thread-";
            threadname += std::to_string(i + 1);
            ThreadData td(threadname);
            // Thread<ThreadData> t(threadname, std::bind(&PthreadPool<T>::ThreadRun, this, std::placeholders::_1), td);
            // _threads.push_back(t);
            _threads.emplace_back(td.GetName(), std::bind(&PthreadPool<T>::ThreadRun, this, std::placeholders::_1), td);
            log.LogMessage(Info, " %s is created successfully!\n", threadname.c_str());
        }
    }

    bool Start()
    {
        // 启动
        for(auto& thread : _threads)
        {
            thread.Start();
            log.LogMessage(Debug, " %s is start successfully!\n", thread.GetThreadName().c_str());
        }
        return true;
    }

    void ThreadWait()
    {
        pthread_cond_wait(&_cond, &_mutex);
    }

    void ThreadWakeup()
    {
        pthread_cond_signal(&_cond);
    }

    void ThreadRun(const ThreadData& td)
    {
        while (true)
        {
            // 提取任务
            T t;
            {
                LockGuard lockguard(&_mutex);
                // 判断队列是否为空，为空则阻塞等待
                while (_q.empty())
                {
                    log.LogMessage(Debug, " %s is waiting ...\n", td.GetName().c_str());
                    ThreadWait();
                    log.LogMessage(Debug, " %s is awake!\n", td.GetName().c_str());
                }
                t = _q.front();
                _q.pop();
            }
            // 运行任务
            t();
            log.LogMessage(Info, " %s is conducting task: %s done, result is: %s\n", \
                td.GetName().c_str(), t.PrintTask().c_str(), t.PrintRet().c_str());
            sleep(1);
        }
    }

    bool IsFull()
    {
        return _q.size() == _pthread_num;
    }

    void Push(T& in)
    {
        while(IsFull())
        {
            log.LogMessage(Debug, " Thread is waiting ...\n");
            // 满了则等待消费
            ThreadWait();
        }
        log.LogMessage(Info, "Thread is pushing task, the task is %s\n", in.PrintTask().c_str());
        // 插入数据并唤醒线程
        LockGuard lockguard(&_mutex);
        _q.push(in);
        ThreadWakeup();
    }

    bool Join()
    {
        for(auto& thread : _threads)
        {
            thread.Join();
        }
        return true;
    }

    ~PthreadPool()
    {
        pthread_mutex_destroy(&_mutex);
        pthread_cond_destroy(&_cond);
    }
private:
    std::queue<T> _q;
    std::vector<Thread<ThreadData>> _threads;
    int _pthread_num;
    pthread_mutex_t _mutex;
    pthread_cond_t _cond;
};