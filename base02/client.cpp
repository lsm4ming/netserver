#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <memory>
#include <unistd.h>
#include <cstring>

constexpr const int MAX_BUFF_SIZE = 4096;

typedef struct ClientHandlerArgs
{
    int client_fd;
    struct sockaddr_in clientaddr;
} ClientHandlerArgs;

void *handlerMessage(void *args)
{
    std::unique_ptr<ClientHandlerArgs> in(reinterpret_cast<ClientHandlerArgs *>(args));

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
        std::cout << "收到服务器消息:" << buf << std::endl;
    }
    return nullptr;
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        std::cout << "缺少启动参数" << std::endl;
        return -1;
    }

    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1)
    {
        perror("socket error");
        return -1;
    }
    struct sockaddr_in *server_addr;
    server_addr->sin_family = AF_INET;                 // ipv4
    server_addr->sin_port = htons(atoi(argv[2]));      // 端口号
    server_addr->sin_addr.s_addr = inet_addr(argv[1]); // ip地址

    // connect
    if (connect(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("connect error");
        return -1;
    }

    // 开启接收线程
    pthread_t pt;
    std::unique_ptr<ClientHandlerArgs> args(new ClientHandlerArgs{socket_fd, *server_addr});
    int ret = pthread_create(&pt, nullptr, handlerMessage, (void *)args.get());
    if (ret < 0)
    {
        perror("pthread_create error");
        return -1;
    }
    pthread_detach(pt);

    while (true)
    {
        std::cout << "请输入发送消息:" << std::endl;
        char buf[MAX_BUFF_SIZE];
        std::cin >> buf;
        if (buf == "exit()")
        {
            std::cout << "bye." << std::endl;
            break;
        }
        write(socket_fd, buf, strlen(buf));
    }
    close(socket_fd);
    return 0;
}