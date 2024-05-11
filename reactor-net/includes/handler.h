#include "common.h"

class ServerHandler
{
public:
    virtual void onAccept(int fd, const sockaddr_in &client_addr) = 0;

    virtual void onReader(int fd) = 0;

    virtual void onWrite(int fd) = 0;

    virtual void onError(int fd) = 0;

    virtual void onExit(int fd) = 0;
};