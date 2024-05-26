#include <iostream>
#include <sw/redis++/redis++.h>

int main()
{
    // docker run --name redis -d -p 6379:6379 redis --requirepass redispass
    using namespace sw::redis;
    ConnectionOptions connection_options;

    connection_options.host = "127.0.0.1";
    connection_options.port = 6379;

    // 设置密码
    connection_options.password = "redispass";

    auto client = Redis(connection_options);
    client.set("name", "lsm1998");
    auto val = client.get("name");
    std::cout << val.value() << std::endl;
    return 0;
}