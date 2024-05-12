#include "server.h"
#include <iostream>

int main(int argc, char *argv[])
{
    Server *server = new ReactorServer("127.0.0.1", 9999);
    server->start();
    return 0;
}