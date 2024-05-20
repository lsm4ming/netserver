#pragma once

#include "common.h"
#include <sys/epoll.h>

constexpr const int MAX_EVENT_LENGTH = 16;

class Epoll
{
private:
    int epoll_fd;
    epoll_event evs[MAX_EVENT_LENGTH];

public:
    Epoll(/* args */);
    ~Epoll();

    int addFd(int fd,uint32_t op);
    Vector<epoll_event> loop(int timeout=-1);
};