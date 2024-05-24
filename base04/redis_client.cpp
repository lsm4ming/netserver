#include <iostream>
#include <sw/redis++/redis++.h>

int main()
{
    using namespace sw::redis;
    auto client = Redis("tcp://127.0.0.1:6379");
    client.set("key", "val");
    auto val = client.get("key");
    return 0;
}