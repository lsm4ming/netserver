#include <sys/fcntl.h>
#include <iostream>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
#include <netinet/tcp.h>

/**
 * epoll工作流程
 *
 * https://zhuanlan.zhihu.com/p/427512269
 */

constexpr const int MAX_BUFF_SIZE = 4096;

// 设置非阻塞的IO
void setNonBlocking(int fd)
{
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

void clientAccept(int listen_fd, int epoll_fd)
{
    sockaddr_in client_addr{};
    memset(&client_addr, 0, sizeof(client_addr));
    socklen_t len = sizeof(client_addr);
    int client_fd = accept(listen_fd, (struct sockaddr *)&client_addr, &len);
    setNonBlocking(client_fd);

    // 加入epoll事件监听
    epoll_event ev{};
    ev.data.fd = client_fd;
    ev.events = EPOLLIN | EPOLLET; // client_fd采用边缘触发
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &ev);
}

void clientClose(int client_fd, int epoll_fd) {}

void clientReader(int client_fd, int epoll_fd)
{
    while (true)
    {
        char buf[MAX_BUFF_SIZE];
        ssize_t n = read(client_fd, buf ,MAX_BUFF_SIZE);
        if(n <0)
        {
            perror("read发生异常");
            break;
        }else if(n == 0)
        {
            clientClose(client_fd, epoll_fd);
            break;
        }
        buf[n] = '\0';
        write(client_fd, buf , n);
        if(n < MAX_BUFF_SIZE)
        {
            break;
        }
    }
}

void clientWriter(int client_fd, int epoll_fd) {}

void clientError(int client_fd, int epoll_fd) {}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        std::cout << "缺少启动参数" << std::endl;
        return -1;
    }

    int listen_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (listen_fd < 0)
    {
        perror("socket() failed");
        return -1;
    }

    // 设置listen_fd的属性
    int opt = 1;
    setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, static_cast<socklen_t>(sizeof opt)); // 必须的。
    setsockopt(listen_fd, SOL_SOCKET, TCP_NODELAY, &opt, static_cast<socklen_t>(sizeof opt));  // 必须的。
    setsockopt(listen_fd, SOL_SOCKET, SO_REUSEPORT, &opt, static_cast<socklen_t>(sizeof opt)); // 有用，但是，在Reactor中意义不大。
    setsockopt(listen_fd, SOL_SOCKET, SO_KEEPALIVE, &opt, static_cast<socklen_t>(sizeof opt)); // 可能有用，但是，建议自己做心跳。

    // 设置非阻塞
    setNonBlocking(listen_fd);

    sockaddr_in server_addr{};
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    server_addr.sin_port = htons(atoi(argv[2]));

    if (bind(listen_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("bind() failed");
        close(listen_fd);
        return -1;
    }

    if (listen(listen_fd, 512) != 0)
    {
        perror("listen() failed");
        close(listen_fd);
        return -1;
    }

    int epoll_fd = epoll_create(1);

    epoll_event ev{};
    ev.data.fd = listen_fd;
    ev.events = EPOLLIN; // listen_fd采用水平触发

    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listen_fd, &ev);

    epoll_event evs[10];

    std::cout << "epoll服务器启动..." << std::endl;

    while (true)
    {
        int in_fds = epoll_wait(epoll_fd, evs, 10, -1);
        if (in_fds < 0)
        {
            perror("epoll_wait() fail");
            break;
        }

        for (int i = 0; i < in_fds; i++)
        {
            if (evs[i].data.fd == listen_fd) // 客户端连接事件
            {
                clientAccept(listen_fd, epoll_fd);
            }
            else
            {
                if (evs[i].events & EPOLLRDHUP) // 客户端关闭事件，部分系统不支持
                {
                    clientClose(evs[i].data.fd, epoll_fd);
                }
                else if (evs[i].events & EPOLLIN | EPOLLPRI) // 客户端可读事件
                {
                    clientReader(evs[i].data.fd, epoll_fd);
                }
                else if (evs[i].events & EPOLLOUT) // 客户端可写事件
                {
                    clientWriter(evs[i].data.fd, epoll_fd);
                }
                else // 错误
                {
                    clientError(evs[i].data.fd, epoll_fd);
                }
            }
        }
    }
    return 0;
}