#include "event.h"

Epoll::Epoll()
{
    this->epoll_fd = epoll_create1(0);
    if (this->epoll_fd < 0)
    {
        perror("epoll_create1() fail");
    }
}

Epoll::~Epoll()
{
    close(this->epoll_fd);
}

void Epoll::updateChannel(Channel *channel)
{
    epoll_event ev;                   // 声明事件的数据结构。
    ev.data.ptr = channel;            // 指定channel。
    ev.events = channel->getEvents(); // 指定事件。

    if (channel->isInpoll()) // 如果channel已经在树上了。
    {
        if (epoll_ctl(this->epoll_fd, EPOLL_CTL_MOD, channel->getFd(), &ev) == -1)
        {
            perror("epoll_ctl() failed.\n");
            exit(-1);
        }
    }
    else // 如果channel不在树上。
    {
        if (epoll_ctl(this->epoll_fd, EPOLL_CTL_ADD, channel->getFd(), &ev) == -1)
        {
            perror("epoll_ctl() failed.\n");
            exit(-1);
        }
        channel->setinepoll(true); // 把channel的inepoll_成员设置为true。
    }
}

void Epoll::removeChannel(Channel *channel)
{
    if (channel->isInpoll()) // 如果channel已经在树上了。
    {
        if (epoll_ctl(this->epoll_fd, EPOLL_CTL_DEL, channel->getFd(), 0) == -1)
        {
            perror("epoll_ctl() failed.\n");
            exit(-1);
        }
    }
}

Vector<Channel *> Epoll::loop(int timeout)
{
    std::vector<Channel *> channels; // 存放epoll_wait()返回的事件。

    bzero(this->evs, sizeof(this->evs));
    int infds = epoll_wait(this->epoll_fd, this->evs, MAX_EVENT_LENGTH, timeout); // 等待监视的fd有事件发生。

    // 返回失败。
    if (infds < 0)
    {
        // EBADF ：epfd不是一个有效的描述符。
        // EFAULT ：参数events指向的内存区域不可写。
        // EINVAL ：epfd不是一个epoll文件描述符，或者参数maxevents小于等于0。
        // EINTR ：阻塞过程中被信号中断，epoll_pwait()可以避免，或者错误处理中，解析error后重新调用epoll_wait()。
        // 在Reactor模型中，不建议使用信号，因为信号处理起来很麻烦，没有必要。------ 陈硕
        perror("epoll_wait() failed");
        exit(-1);
    }

    // 超时。
    if (infds == 0)
    {
        // 如果epoll_wait()超时，表示系统很空闲，返回的channels将为空。
        return channels;
    }

    // 如果infds>0，表示有事件发生的fd的数量。
    for (int ii = 0; ii < infds; ii++) // 遍历epoll返回的数组events_。
    {
        Channel *ch = (Channel *)this->evs[ii].data.ptr; // 取出已发生事件的channel。
        ch->setrevents(this->evs[ii].events);            // 设置channel的revents_成员。
        channels.push_back(ch);
    }
    return channels;
}
