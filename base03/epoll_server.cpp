#include <sys/fcntl.h>
#include <iostream>

// 设置非阻塞的IO
void setnonblocking(int fd)
{
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        std::cout << "缺少启动参数" << std::endl;
        return -1;
    }
    return 0;
}