#pragma once

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

class InetAddr
{
public:
    InetAddr(struct sockaddr_in peer)
        :_peer(peer)
    {
        _ip = inet_ntoa(_peer.sin_addr);
        _port = ntohs(_peer.sin_port);
    }

    std::string Info()
    {
        std::string info = _ip;
        info += ":";
        info += std::to_string(_port);

        return info;
    }

    std::string R_IP()  {return _ip;}

    uint64_t R_PORT()  {return _port;}

    ~InetAddr(){}
private:
    std::string _ip;
    uint64_t _port;
    struct sockaddr_in _peer;
};