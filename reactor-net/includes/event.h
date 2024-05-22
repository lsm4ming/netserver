#pragma once

#include "common.h"
#include "channel.h"
#include <sys/epoll.h>

constexpr const int MAX_EVENT_LENGTH = 128;

class Epoll
{
private:
    int epoll_fd{-1};
    epoll_event evs[MAX_EVENT_LENGTH];

public:
    Epoll();
    ~Epoll();

    void updateChannel(Channel *channel);
    void removeChannel(Channel *channel);
    Vector<Channel*> loop(int timeout=-1);
};