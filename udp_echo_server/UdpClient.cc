#include <iostream>
#include <cerrno>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Log.hpp"
#include "Command.hpp"

void Usage(const char* process)
{
    std::cout << "Usage:\n\t" << process << " server_ip " << "server_port\n" << std::endl;
}

int main(int argc, char* argv[])
{
   
    if (argc != 3)
    {
        Usage(argv[0]);
        return Usage_error;
    }

    // 从命令行获取ip和端口号
    std::string server_ip = argv[1];
    uint64_t server_port = std::stoi(argv[2]);

    // 接收消息给服务器
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        log.LogMessage(Fatal, "socket is created failed! Error is %d, %s\n", errno, strerror(errno));
        exit(Socket_error);
    }
    log.LogMessage(Info, "Socket is created successfully!\n");

    // 需要bind数据，但是不需要自己bind，客户端的信息在随时变化
    // client会在首次发送数据的时候会自动进行bind
    // 填充server信息
    struct sockaddr_in server;
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(server_port);
    server.sin_addr.s_addr = inet_addr(server_ip.c_str());

    while (true)
    {
        // 我们所要发送的数据
        std::string inbuffer;
        std::cout << "Please Enter# ";
        std::getline(std::cin, inbuffer);

        // 发消息给谁，发给服务器server
        ssize_t n = sendto(sockfd, inbuffer.c_str(), inbuffer.size(), 0, (struct sockaddr*)&server, sizeof(server));
        if (n > 0)
        {
            struct sockaddr_in tmp;
            socklen_t len = sizeof(tmp);
            char buffer[1024];
            ssize_t m = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0, (struct sockaddr*)&tmp, &len);
            if (m > 0)
            {
                buffer[m] = 0;
                std::cout << "Server echo# " << buffer << std::endl;
            }
            else break;
        }
        else break;
    }
    return 0;
}