#pragma once 

#include <pthread.h>

class LockGuard
{
public:
    LockGuard(pthread_mutex_t* mutex):_mutex(mutex)
    {
        pthread_mutex_lock(_mutex);
    }

    ~LockGuard()
    {
        pthread_mutex_unlock(_mutex);
    }
private:
    pthread_mutex_t* _mutex;
};

// 默认不定义锁，等待外部传入锁
// class Mutex
// {
// public:
//     Mutex(pthread_mutex_t* lock):_lock(lock)
//     {}

//     void Lock()
//     {
//         pthread_mutex_lock(_lock);
//     }

//     void Unlock()
//     {
//         pthread_mutex_unlock(_lock);
//     }

//     ~Mutex()
//     {}
// private:
//     pthread_mutex_t* _lock;
// };

// class LockGuard
// {
//     LockGuard(Mutex mutex):_mutex(mutex)
//     {
//         _mutex.Lock();
//     }

//     ~LockGuard()
//     {
//         _mutex.Unlock();
//     }
// private:
//     Mutex _mutex;
// };