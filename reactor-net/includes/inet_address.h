#pragma once

#include "common.h"
#include <arpa/inet.h>
#include <netinet/tcp.h>

class InetAddress
{
private:
    sockaddr_in addr_;

public:
    InetAddress();
    InetAddress(const std::string &ip, uint16_t port);
    InetAddress(const sockaddr_in addr);
    ~InetAddress();

    const char *ip() const;
    uint16_t port() const;
    const sockaddr *addr() const;
    void setaddr(sockaddr_in client_addr);
};