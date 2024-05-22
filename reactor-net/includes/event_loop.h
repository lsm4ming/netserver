#pragma once

#include "common.h"

class Channel;

class EventLoop
{
public:
    void updatechannel(Channel *ch); // 把channel添加/更新到红黑树上，channel中有fd，也有需要监视的事件。
    void removechannel(Channel *ch); // 从黑树上删除channel。
};