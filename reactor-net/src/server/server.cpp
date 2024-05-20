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

    Epoll epoll;

    epoll.addFd(listen_fd, EPOLLIN); // listen_fd采用水平触发

    std::cout << "reactorServer start..." << std::endl;
    while (true)
    {
        Vector<epoll_event> list = epoll.loop();
        if (list.empty())
        {
            continue;
        }
        std::cout << "收到" << list.size() << "个事件" << std::endl;
        for (auto &item : list)
        {
            if (item.data.fd == listen_fd) // 客户端连接事件
            {
                sockaddr_in client_addr{};
                socklen_t len = sizeof(client_addr);
                int client_fd = accept(listen_fd, (struct sockaddr *)&client_addr, &len);
                // 设置非阻塞
                Socket::setNonBlocking(client_fd);
                // 加入epoll事件监听
                epoll.addFd(client_fd, EPOLLIN | EPOLLET);
                if (this->handler)
                {
                    this->handler->onAccept(client_fd, client_addr);
                }
            }
            else
            {
                if (item.events & EPOLLRDHUP && this->handler) // 客户端关闭事件，部分系统不支持
                {
                    this->handler->onExit(item.data.fd);
                }
                else if (item.events & EPOLLIN | EPOLLPRI && this->handler) // 客户端可读事件
                {
                    this->handler->onReader(item.data.fd);
                }
                else if (item.events & EPOLLOUT && this->handler) // 客户端可写事件
                {
                    this->handler->onWrite(item.data.fd);
                }
                else if (this->handler) // 错误
                {
                    this->handler->onError(item.data.fd);
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
