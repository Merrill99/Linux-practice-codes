#include <string>
#include <ctime>
#include <unistd.h>
#include "RingQueue.hpp"
#include "Task.hpp"
#include "Log.hpp"


struct ThreadData
{
    RingQueue<Task>* _rq;
    std::string _name;
};

void* consumer(void* args)
{
    ThreadData* td = static_cast<ThreadData*>(args);
    while (true)
    {
        Task t;
        // 消费者从队列拿数据
        td->_rq->Pop(&t);
        // 任务运行
        t();
        // 打印消费者任务
        cout << "consumer's data: " << t.PrintRet() << "," << td->_name << endl;
        sleep(1);
    }
}

void* productor(void* args)
{
    while (true)
    {
        RingQueue<Task>* rq = static_cast<RingQueue<Task>*>(args);
        int data1  = rand() % 1000;
        usleep(rand() % 500);
        int data2  = rand() % 1000;
        usleep(rand() % 500);
        char sy = symbols[rand() % symbols.size()];
        Task t(data1, data2, sy);
        // 打印一下生产者的任务
        cout << "productor's task: " << t.PrintTask() << endl;
        rq->Push(t);
        // sleep(1);
    }
}

int main()
{   
    // 生成随机数
    srand((time_t)time(nullptr) ^ getpid());
    // srand((uint16_t)time(nullptr) ^ getpid() ^ pthread_self()); 

    // 三个消费者，两个生产者
    pthread_t c[3], p[2];
    
    // 创建循环队列
    RingQueue<Task>* rq = new RingQueue<Task>();

    // 创建消费者属性
    ThreadData* td1 = new ThreadData();
    td1->_rq = rq;
    td1->_name = "thread-1";
    pthread_create(&c[0], nullptr, consumer, td1);
    

    ThreadData* td2 = new ThreadData();
    td2->_rq = rq;
    td2->_name = "thread-2";
    pthread_create(&c[1], nullptr, consumer, td2);
    

    ThreadData* td3 = new ThreadData();
    td3->_rq = rq;
    td3->_name = "thread-3";
    pthread_create(&c[2], nullptr, consumer, td3);
    

    // 创建生产者属性
    pthread_create(&p[0], nullptr, productor, rq);
    pthread_create(&p[1], nullptr, productor, rq);

    pthread_join(c[0], nullptr);
    pthread_join(c[1], nullptr);
    pthread_join(c[2], nullptr);
    pthread_join(p[0], nullptr);
    pthread_join(p[1], nullptr);

    return 0;
}