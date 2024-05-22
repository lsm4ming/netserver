#include "channel.h"

Channel::Channel(EventLoop *loop, int fd)
{
    this->loop = loop;
    this->fd = fd;
}

Channel::~Channel()
{
}

int Channel::getFd() const
{
    return this->fd;
}

void Channel::useet()
{
    this->events |= EPOLLET;
}

void Channel::enablereading()
{
    this->events |= EPOLLIN;
    this->loop->updatechannel(this);
}

void Channel::disablereading()
{
    this->events &= ~EPOLLIN;
    this->loop->updatechannel(this);
}

void Channel::enablewriting()
{
    this->events |= EPOLLOUT;
    this->loop->updatechannel(this);
}

void Channel::disablewriting()
{
    this->events &= ~EPOLLOUT;
    this->loop->updatechannel(this);
}

void Channel::disableall()
{
    this->events = 0;
    this->loop->updatechannel(this);
}

void Channel::remove()
{
    // 先取消全部的事件
    this->disableall();
    // 从红黑树上删除fd
    this->loop->removechannel(this);
}

void Channel::setinepoll(bool inepoll)
{
    this->inepoll = inepoll;
}

void Channel::setrevents(uint32_t ev)
{
    this->revents = ev;
}

bool Channel::isInpoll() const
{
    return this->inepoll;
}

uint32_t Channel::getEvents() const
{
    return this->events;
}

uint32_t Channel::getRevents() const
{
    return this->revents;
}

void Channel::handleEvent()
{
    if (this->revents & EPOLLRDHUP) // 对方已关闭，有些系统检测不到，可以使用EPOLLIN，recv()返回0。
    {
        closecallback(); // 回调Connection::closecallback()。
    }
    else if (this->revents & (EPOLLIN | EPOLLPRI)) // 接收缓冲区中有数据可以读。
    {
        readcallback(); // 如果是acceptchannel，将回调Acceptor::newconnection()，如果是clientchannel，将回调Connection::onmessage()。
    }
    else if (this->revents & EPOLLOUT) // 有数据需要写。
    {
        writecallback(); // 回调Connection::writecallback()。
    }
    else // 其它事件，都视为错误。
    {
        errorcallback(); // 回调Connection::errorcallback()。
    }
}

void Channel::setreadcallback(std::function<void()> fn)
{
    this->readcallback = fn;
}

void Channel::setclosecallback(std::function<void()> fn)
{
    this->closecallback = fn;
}

void Channel::seterrorcallback(std::function<void()> fn)
{
    this->errorcallback = fn;
}

void Channel::setwritecallback(std::function<void()> fn)
{
    this->writecallback = fn;
}
