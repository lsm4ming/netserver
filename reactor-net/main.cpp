#include "server.h"
#include "poll.h"
#include <iostream>

int main(int argc, char *argv[])
{
    Epoll poll{};
    poll.addFd(1, 1);
    poll.loop();

    Server *server = new ReactorServer("127.0.0.1", 9999);
    server->start();
    return 0;
}