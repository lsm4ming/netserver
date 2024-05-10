#pragma once

#include "common.h"
#include "inet_address.h"

class Socket
{
private:
    const int fd_;

public:
    Socket(int fd) : fd_(fd){};
    ~Socket();

    int fd() const;
    void setReuseAddr(bool on = false);
    void setReusePort(bool on = false);
    void setTcpNodelay(bool on = false);
    void setKeepalive(bool on = false);
    int bind(const InetAddress &server_addr);
    int listen(int n = 128);
    int accept(InetAddress &client_addr);
    static int createNonBlocking();
};