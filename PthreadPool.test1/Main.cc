#include "PthreadPool.hpp"
#include "Task.hpp"

int main()
{
    // PthreadPool<int> tp;
    // tp.Start();
    // tp.Join();

    std::unique_ptr<PthreadPool<Task>> tp(new PthreadPool<Task>());
    tp->Start();
    srand((time_t)time(nullptr) ^ getpid());
    
    while (true)
    {
        int x = rand() % 500;
        usleep(rand() % 100);
        int y = rand() % 500;
        usleep(rand() % 100);

        char sy = symbols[rand() % symbols.size()];
        // 创建任务
        Task t(x, y, sy);
        // 加入任务
        
        tp->Push(t);
        sleep(1);
    }

    tp->Join();
    return 0;
}