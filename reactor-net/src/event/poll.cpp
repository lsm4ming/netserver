#include "poll.h"

Epoll::Epoll()
{
    this->epoll_fd = epoll_create1(1);
}

Epoll::~Epoll()
{
    close(this->epoll_fd);
}

int Epoll::addFd(int fd, uint32_t op)
{
    epoll_event ev{};
    ev.data.fd = fd;
    ev.events = op;
    return epoll_ctl(this->epoll_fd, EPOLL_CTL_ADD, fd, &ev);
}

Vector<epoll_event> Epoll::loop(int timeout)
{
    bzero(this->evs, sizeof(this->evs));
    int in_fds = epoll_wait(this->epoll_fd, this->evs, MAX_EVENT_LENGTH, timeout);
    if (in_fds < 0)
    {
        perror("epoll_wait() fail");
        return Vector<epoll_event>();
    }
    else if (in_fds == 0)
    {
        return Vector<epoll_event>();
    }
    Vector<epoll_event> list;
    for (int i = 0; i < in_fds; i++)
    {
        list.push_back(this->evs[i]);
    }
    return list;
}
