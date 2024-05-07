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

// 设置非阻塞的IO
void setnonblocking(int fd)
{
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

void clientAccept(int listenfd, int epoll_fd)
{
    struct sockaddr_in clientaddr;
    memset(&clientaddr, 0, sizeof(clientaddr));
    socklen_t len = sizeof(clientaddr);
    int client_fd = accept(listenfd, (struct sockaddr *)&clientaddr, &len);
    setnonblocking(client_fd);

    // 加入epoll事件监听
    epoll_event ev{};
    ev.data.fd = client_fd;
    ev.events = EPOLLIN | EPOLLET; // client_fd采用边缘触发
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listenfd, &ev);
}

void clientClose(int client_fd, int epoll_fd) {}

void clientReader(int client_fd, int epoll_fd) {}

void clientWriter(int client_fd, int epoll_fd) {}

void clientError(int client_fd, int epoll_fd) {}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        std::cout << "缺少启动参数" << std::endl;
        return -1;
    }

    int listenfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listenfd < 0)
    {
        perror("socket() failed");
        return -1;
    }

    // 设置listenfd的属性
    int opt = 1;
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, static_cast<socklen_t>(sizeof opt)); // 必须的。
    setsockopt(listenfd, SOL_SOCKET, TCP_NODELAY, &opt, static_cast<socklen_t>(sizeof opt));  // 必须的。
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEPORT, &opt, static_cast<socklen_t>(sizeof opt)); // 有用，但是，在Reactor中意义不大。
    setsockopt(listenfd, SOL_SOCKET, SO_KEEPALIVE, &opt, static_cast<socklen_t>(sizeof opt)); // 可能有用，但是，建议自己做心跳。

    // 设置非阻塞
    setnonblocking(listenfd);

    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(argv[1]);
    servaddr.sin_port = htons(atoi(argv[2]));

    if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("bind() failed");
        close(listenfd);
        return -1;
    }

    if (listen(listenfd, 512) != 0)
    {
        perror("listen() failed");
        close(listenfd);
        return -1;
    }

    int epoll_fd = epoll_create(1);

    epoll_event ev{};
    ev.data.fd = listenfd;
    ev.events = EPOLLIN; // listenfd采用水平触发

    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listenfd, &ev);

    epoll_event evs[10];
    while (true)
    {
        int infds = epoll_wait(epoll_fd, evs, 10, -1);
        if (infds < 0)
        {
            perror("epoll_wait() fail");
            break;
        }

        for (int i = 0; i < infds; i++)
        {
            if (evs[i].data.fd == listenfd) // 客户端连接事件
            {
                clientAccept(listenfd, epoll_fd);
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