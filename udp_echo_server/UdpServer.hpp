#pragma once

#include <iostream>
#include <cstring>
#include <cerrno>
#include <cstdlib>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "nocopy.hpp"
#include "Log.hpp"
#include "Command.hpp"
#include "InetAddr.hpp"

static const std::string default_ip = "0.0.0.0";
static const uint64_t default_port = 8888;
static const int defualt_fd = -1;
static const int defualt_size = 1024;

class udp_server : public nocopy
{
public:
    /*udp_server(const std::string ip = default_ip, const uint64_t port = default_port, \
        int fd = defualt_fd)
        :_ip(ip), _port(port), _sockfd(fd)
    {}*/

    udp_server(const uint64_t port = default_port)
        :_port(port), _sockfd(defualt_fd)
    {}
    // 初始化
    void Init()
    {
        // 1. 创建套接字
        _sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (_sockfd < 0)
        {
            log.LogMessage(Fatal, "Socket is created error! error is %d, %s\n", errno, strerror(errno));
            exit(Socket_error);
        }
        log.LogMessage(Info, "Socket is created successfully! Sockfd is %d .\n", _sockfd);

        // 2. 绑定，指定网络信息
        struct sockaddr_in local;
        bzero(&local, sizeof(local));
        local.sin_family = AF_INET;
        local.sin_port = htons(_port);
        // local.sin_addr.s_addr = inet_addr(_ip.c_str());// 4字节ip变成网络序列
        local.sin_addr.s_addr = INADDR_ANY;

        int n = bind(_sockfd, (struct sockaddr*)&local, sizeof(local));
        if (n != 0)
        {
            log.LogMessage(Fatal, "Bind is error! Error is %d, %s", errno, strerror(errno));
            exit(Bind_error);
        }
    }
    // 开始运行
    void Start()
    {
        // 服务器永远不退出
        char buffer[defualt_size];
        while (true)
        {
            struct sockaddr_in peer;
            socklen_t len = sizeof(peer); // 不能乱写
            // 把接收到的消息作为字符串来看，预留一个'\0'
            ssize_t n = recvfrom(_sockfd, buffer, sizeof(buffer) - 1, 0, (struct sockaddr*)&peer, &len);
            // 不做错误判断
            if (n > 0)
            {
                // 获取客户端的信息
                /*uint64_t port = ntohs(peer.sin_port);
                std::string ip = inet_ntoa(peer.sin_addr);
                std::string info = ip;
                info += ":";
                info += std::to_string(port);*/

                InetAddr ia(peer);

                buffer[n] = 0; // 使字符串最后一个字符以'\0'收尾
                sendto(_sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&peer, len);
                std::cout << "[" << ia.Info() << "]# " << buffer << std::endl;
            }
        }
    }
    ~udp_server(){}
private:
    // std::string _ip; // 默认ip地址，先写着，后面是不需要的
    uint64_t _port; // 默认端口号
    int _sockfd; 
};