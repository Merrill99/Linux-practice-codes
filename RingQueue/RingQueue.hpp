#pragma once

#include <iostream>
#include <vector>
#include <pthread.h>
#include <semaphore.h>
#include "LockGuard.hpp"

const int defualtsize = 5;

// 基于环形队列的生产者消费者模型
// 使用POSIX信号量！
template<class T>
class RingQueue
{
// 信号操作的封装
private:
    // 信号的等待，P操作
    void P(sem_t& sem)
    {
        sem_wait(&sem);
    }

    // 信号的释放，V操作
    void V(sem_t& sem)
    {
        sem_post(&sem);
    }
public:
    RingQueue(int size = defualtsize)
        :_rq(size), _size(size), _c_step(0), _p_step(0)
    {
        // 初始化锁
        pthread_mutex_init(&_c_mutex, nullptr);
        pthread_mutex_init(&_p_mutex, nullptr);
        // 初始化信号量
        sem_init(&_space_sem, 0, _size);
        sem_init(&_data_sem, 0, 0);
    }

    void Push(const T& data)
    {
        P(_space_sem);
        {
            LockGuard lockguard(&_p_mutex);
            // pthread_mutex_lock(&_p_mutex);
            _rq[_p_step] = data;
            ++_p_step;
            _p_step %= _size;
            // pthread_mutex_unlock(&_c_mutex);
        }
        V(_data_sem);
    }

    void Pop(T* output)
    {
        P(_data_sem);
        {
            LockGuard lockguard(&_c_mutex);
            // pthread_mutex_lock(&_c_mutex);
            *output = _rq[_c_step];
            ++_c_step;
            _c_step %= _size;
            // pthread_mutex_unlock(&_c_mutex);
        }
        V(_space_sem);
    }

    ~RingQueue()
    {
        // 销毁锁
        pthread_mutex_destroy(&_c_mutex);
        pthread_mutex_destroy(&_p_mutex);
        // 销毁信号量
        sem_destroy(&_data_sem);
        sem_destroy(&_space_sem);
    }

private:
    std::vector<T> _rq;

    int _size;

    int _c_step; // 消费者的位置
    int _p_step; // 生产者的位置

    pthread_mutex_t _c_mutex; // 消费者锁
    pthread_mutex_t _p_mutex; // 生产者锁

    sem_t _data_sem; // 消费者信号量
    sem_t _space_sem; // 生产者信号量
};