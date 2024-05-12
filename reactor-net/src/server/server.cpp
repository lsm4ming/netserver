#include "server.h"

void ReactorServer::setHost(const String &host)
{
    this->host = host;
}

void ReactorServer::setPort(u_int16_t port)
{
    this->port = port;
}

void ReactorServer::start()
{
    int listen_fd = Socket::createNonBlocking();
    if (listen_fd < 0)
    {
        exit(listen_fd);
    }

    Socket server_socket(listen_fd);
    server_socket.setKeepalive(true);
    server_socket.setReuseAddr(true);
    server_socket.setReusePort(true);
    server_socket.setTcpNodelay(true);

    InetAddress address(this->host, this->port);

    int exitCode;
    if ((exitCode = server_socket.bind(address)) < 0)
    {
        exit(exitCode);
    }
    if ((exitCode = server_socket.listen()) < 0)
    {
        exit(exitCode);
    }

    int epoll_fd = epoll_create1(0);
    if (epoll_fd < 0)
    {
        perror("epoll_create1() fail");
        exit(epoll_fd);
    }

    struct epoll_event ev{};
    ev.data.fd = listen_fd;
    ev.events = EPOLLIN; // listen_fd采用水平触发

    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listen_fd, &ev);

    epoll_event evs[16];

    std::cout << "reactorServer start..." << std::endl;
    while (true)
    {
        int in_fds = epoll_wait(epoll_fd, evs, 16, -1);
        if (in_fds < 0)
        {
            perror("epoll_wait() fail");
            break;
        }
        std::cout << "收到" << in_fds << "个事件" << std::endl;
        for (int i = 0; i < in_fds; i++)
        {
            if (evs[i].data.fd == listen_fd) // 客户端连接事件
            {
                sockaddr_in client_addr{};
                socklen_t len = sizeof(client_addr);
                int client_fd = accept(listen_fd, (struct sockaddr *) &client_addr, &len);
                // 设置非阻塞
                Socket::setNonBlocking(client_fd);
                // 加入epoll事件监听
                epoll_event ev{};
                ev.data.fd = client_fd;
                ev.events = EPOLLIN | EPOLLET; // client_fd采用边缘触发
                epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &ev);
                if (this->handler)
                {
                    this->handler->onAccept(client_fd, client_addr);
                }
            } else
            {
                if (evs[i].events & EPOLLRDHUP && this->handler) // 客户端关闭事件，部分系统不支持
                {
                    this->handler->onExit(evs[i].data.fd);
                } else if (evs[i].events & EPOLLIN | EPOLLPRI && this->handler) // 客户端可读事件
                {
                    this->handler->onReader(evs[i].data.fd);
                } else if (evs[i].events & EPOLLOUT && this->handler) // 客户端可写事件
                {
                    this->handler->onWrite(evs[i].data.fd);
                } else if (this->handler) // 错误
                {
                    this->handler->onError(evs[i].data.fd);
                }
            }
        }
    }
}

void ReactorServer::stop()
{
    this->running = false;
}

void ReactorServer::setHandler(ServerHandler *handler)
{
    this->handler = handler;
}
