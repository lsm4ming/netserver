#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <netinet/tcp.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/fcntl.h>
#include <pthread.h>
#include <memory>

constexpr const int MAX_BUFF_SIZE = 4096;

typedef struct ClientHandlerArgs
{
    int client_fd;
    struct sockaddr_in clientaddr;
} ClientHandlerArgs;

// 设置非阻塞的IO
void setnonblocking(int fd)
{
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

void *clientHandler(void *args)
{
    std::unique_ptr<ClientHandlerArgs> in(reinterpret_cast<ClientHandlerArgs *>(args)); // 使用智能指针进行内存管理

    char buf[MAX_BUFF_SIZE];

    while (true)
    {
        int n = read(in->client_fd, buf, MAX_BUFF_SIZE);
        if (n < 0)
        {
            perror("read client_fd failed");
            break;
        }
        else if (n == 0)
        {
            std::cout << "连接退出" << std::endl;
            break;
        }
        buf[n] = '\0';
        write(in->client_fd, buf, n);
    }

    close(in->client_fd);
    return nullptr;
}

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

    std::cout << "开始监听..." << std::endl;

    while (true)
    {
        struct sockaddr_in clientaddr;
        memset(&clientaddr, 0, sizeof(clientaddr));
        socklen_t len = sizeof(clientaddr);
        int client_fd = accept(listenfd, (struct sockaddr *)&clientaddr, &len);
        if (client_fd < 0)
        {
            // 休眠500毫秒
            usleep(500 * 1000);
            continue;
        }

        pthread_t pt;

        // 创建新线程处理连接
        std::unique_ptr<ClientHandlerArgs> args(new ClientHandlerArgs{client_fd, clientaddr});
        int ret = pthread_create(&pt, nullptr, clientHandler, (void *)args.get());
        if (ret < 0)
        {
            perror("pthread_create error");
            close(client_fd);
            continue;
        }
        args.release(); // 释放所有权，避免在线程中释放
        pthread_detach(pt);
    }
    return 0;
}