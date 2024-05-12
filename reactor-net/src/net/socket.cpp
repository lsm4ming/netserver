#include "socket.h"

Socket::~Socket()
{
    ::close(this->fd_);
}

int Socket::fd() const
{
    return this->fd_;
}

void Socket::setReuseAddr(bool on)
{
    int optval = on ? 1 : 0;
    ::setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
}

void Socket::setReusePort(bool on)
{
    int optval = on ? 1 : 0;
    ::setsockopt(fd_, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval));
}

void Socket::setTcpNodelay(bool on)
{
    int optval = on ? 1 : 0;
    ::setsockopt(fd_, IPPROTO_TCP, TCP_NODELAY, &optval, sizeof(optval));
}

void Socket::setKeepalive(bool on)
{
    int optval = on ? 1 : 0;
    ::setsockopt(fd_, SOL_SOCKET, SO_KEEPALIVE, &optval, sizeof(optval));
}

int Socket::bind(const InetAddress &server_addr)
{
    if (::bind(fd_, server_addr.addr(), sizeof(sockaddr)) < 0)
    {
        perror("bind() failed");
        return ERROR_BIND_SOCKET;
    }
    return OK;
}

int Socket::listen(int n)
{
    if (::listen(fd_, n) != 0)
    {
        perror("listen() failed");
        return ERROR_LISTEN_SOCKET;
    }
    return OK;
}

int Socket::accept(InetAddress &client_addr)
{
    sockaddr_in peera_ddr;
    socklen_t len = sizeof(peera_ddr);
    int client_fd = accept4(fd_, (sockaddr *)&peera_ddr, &len, SOCK_NONBLOCK);
    client_addr.setaddr(peera_ddr);
    return client_fd;
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

void Socket::setNonBlocking(int fd)
{
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}
