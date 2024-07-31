#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <cstdio>
#include <pthread.h>
#include <unistd.h>

using namespace std;

template<class T>
using func_t = function<void(T&)>;

template<class T>
class Thread
{
public:
    Thread(const string& pthreadname, func_t<T&> func, T& data)
        :_tid(0)
        ,_pthreadname(pthreadname)
        ,_isrunning(false)
        ,_func(func)
        ,_data(data)
    {}

    static void *ThreadRoutine(void *args)
    {
        // (void)args; // 仅仅是为了防止编译器告警

        Thread* This = static_cast<Thread*>(args);
        // 新线程使用函数func
        This->_func(This->_data);

        return nullptr;
    }

    bool Start()
    {
        int n = pthread_create(&_tid, nullptr, ThreadRoutine, this);
        if (n == 0) // 成功创建
        {
            _isrunning = true;
            return true;
        }
        else return false;
    }

    bool Join()
    {
        if (!_isrunning) return true; // 没有运行直接join
        int n = pthread_join(_tid, nullptr);
        if (n == 0) 
        {
            _isrunning = false;
            return true;
        }
        return false;
    }

    const string& GetThreadName()
    {
        return _pthreadname;
    }

    bool IsRunning()
    {
        return _isrunning;
    }

    ~Thread()
    {}
private:
    pthread_t _tid;
    string _pthreadname;
    bool _isrunning;
    func_t<T&> _func;
    T& _data;
};