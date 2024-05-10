#include "socket.h"

Socket::~Socket()
{
    close(this->fd_);
}

int Socket::fd() const
{
    return this->fd_;
}

void Socket::setReuseAddr(bool on)
{
}

void Socket::setReusePort(bool on)
{
}

void Socket::setTcpNodelay(bool on)
{
}

void Socket::setKeepalive(bool on)
{
}

void Socket::bind(const InetAddress &server_addr)
{
}

void Socket::listen(int n)
{
}

int Socket::accept(InetAddress &client_addr)
{
    return 0;
}

int Socket::createNonBlocking()
{
    int listenfd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, IPPROTO_TCP);
    if (listenfd < 0)
    {
        perror("socket create error");
        return ERROR_CREATE_SOCKET_FD;
    }
    return listenfd;
}
