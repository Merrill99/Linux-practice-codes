#include <iostream>
#include <cassert>
#include <vector>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "ProcessPool.hpp"

using namespace std;

const int count = 5;
static int number = 1;

// 信道
class channel
{
public:
    channel(int fd, pid_t id)
        :ctrlfd(fd), 
        workerid(id)
        {
            name = "channel-" +  to_string(number++);
        }
public:
    int ctrlfd; // 控制描述符
    pid_t workerid; // 判断是哪一个进程提供处理的
    string name;
};

void Work()
{
    while (1)
    {
        // cout << "I am running:" << getpid() << endl;
        // sleep(1);

        int code = 0;
        ssize_t n = read(0, &code, sizeof(code));
        if (n == sizeof(code))
        {
            if (!init.CheckSafe(code)) continue;
            init.RunTasks(code);
        }
        else if (n == 0)
        {
            break;
        }
        else
        {
            // do nothing
        }
   }
   cout << "child quit" << endl;
}

void CreatChannel(vector<channel> *c)
{
    // // v1
    // // bug,除了最后一个管道，其余管道会有多个写端指向
    // for (int i = 0; i < count; ++i)
    // {
    //     // 1. 定义并创建管道
    //     int pipefd[2] = { 0 };
    //     int n = pipe(pipefd);
    //     assert(n == 0);
    //     (void)n;

    //     // 2. 创建进程
    //     pid_t id = fork();
    //     assert(id != -1);

    //     // 3. 构建单向通信的信道
    //     if (id == 0) // child
    //     {
    //         // 关闭写管道
    //         close(pipefd[1]);
    //         // TODO
    //         dup2(pipefd[0], 0); // 将标准输入重定向到管道的读端
    //         Work(); // 管道要读数据直接从标准输入中读取，不用传参
    //         exit(0); // 会自动关闭自己打开的所有fd
    //     }

    //     // father
    //     // 关闭读管道
    //     close(pipefd[0]);
    //     c->push_back(channel(pipefd[1], id));
    // }

    // v2
    vector<int> temp;
    for (int i = 0; i < count; ++i)
    {
        // 1. 定义并创建管道
        int pipefd[2] = { 0 };
        int n = pipe(pipefd);
        assert(n == 0);
        (void)n;

        // 2. 创建进程
        pid_t id = fork();
        assert(id != -1);

        // 3. 构建单向通信的信道
        if (id == 0) // child
        {
            if (!temp.empty())
            {
                for (auto e : temp)
                {
                    close(e);
                }
            }
            // 关闭写管道
            close(pipefd[1]);
            // TODO
            dup2(pipefd[0], 0); // 将标准输入重定向到管道的读端
            Work(); // 管道要读数据直接从标准输入中读取，不用传参
            exit(0); // 会自动关闭自己打开的所有fd
        }

        // father
        // 关闭读管道
        close(pipefd[0]);
        c->push_back(channel(pipefd[1], id));
        temp.push_back(pipefd[1]);
    }
}

void PrintTest(const vector<channel> &c)
{
    for (const auto &e : c)
    {
        cout << e.name << "," << e.ctrlfd << "," << e.workerid << endl;
    }
}

void SendTask(const vector<channel> &channels, bool flag, int num = -1)
{
    int pos = 0;
    while (1)
    {
        // 1. 选择任务
        int command = init.SelectTask();

        // 2. 选择信道（进程）
        const auto &c = channels[pos++];
        pos %= channels.size();

        // debug
        cout << "send command " << init.ToName(command) << "[" << command << "]" << 
            "to " << c.name << " worker is:" << c.workerid << endl;

        // 3. 发送任务
        write(c.ctrlfd, &command, sizeof(command));

        // 4. 判断是否要退出
        if (!flag)
        {
            if (num-- <= 0)
                break;
        }
    }

    cout << "sendcommand done ..." << endl;
}

void ReleaseChannels(vector<channel> channels)
{
    // v1
    // for (const auto &channel : channels)
    // {
    //     close(channel.ctrlfd);
    // }

    // for (const auto &channel : channels)
    // {
    //     pid_t wid = waitpid(channel.workerid, nullptr, 0);
    //     if (wid == channel.workerid)
    //     {
    //         cout << "wait child:" << channel.workerid << ", success" << endl;
    //     }
    // }

    // v2 倒着回收
    int i = channels.size() - 1;
    for (; i >= 0; --i)
    {
        close(channels[i].ctrlfd);
        waitpid(channels[i].workerid, nullptr, 0);
    }

}

int main()
{
    vector<channel> channels;
    // 1. 创建信道
    CreatChannel(&channels);

    // 2. 开始发送任务
    const bool always_loop = true;
    SendTask(channels, !always_loop, 10);

    // 3. 回收资源，想让子进程退出，并且释放管道，只要关闭写端即可
    ReleaseChannels(channels);
    return 0;
}
