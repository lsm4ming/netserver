#pragma once

#include "common.h"
#include "server.h"
#include "socket.h"
#include "handler.h"
#include <sys/epoll.h>

class Server
{
public:
    virtual void start() = 0;

    virtual void stop() = 0;
};

class ReactorServer : public Server
{
public:
    ReactorServer(String host = "127.0.0.1", u_int16_t port = 8080) : host(std::move(host)), port(port) {}

    ~ReactorServer() = default;

    void setHost(const String &host);

    void setPort(u_int16_t port);

    void start() override;

    void stop() override;

    void setHandler(ServerHandler *handler);

private:
    String host;
    u_int16_t port;
    bool running{true};
    ServerHandler *handler{};
};