#include <memory>
#include "UdpServer.hpp"
#include "Command.hpp"

void Usage(std::string proc)
{
    std::cout << "Usage: \n\t" << proc << " local_proc \n" << std::endl;
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        Usage(argv[0]); // 调用使用手册
        return Usage_error;
    }

    // std::string ip = argv[1]; // 接收输入的ip地址
    // uint64_t port = std::stoi(argv[2]); // 接收输入的端口号
    uint64_t port = std::stoi(argv[1]);
    
    std::unique_ptr<udp_server> usrv(new udp_server());
    usrv->Init();
    usrv->Start();
    return 0;
}