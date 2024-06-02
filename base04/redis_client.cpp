#include <iostream>
#include <sw/redis++/redis++.h>

constexpr const char *REDIS_HOST = "192.168.239.50";
constexpr const int REDIS_PORT = 6379;
constexpr const char *REDIS_PASS = "redispass";

int main()
{
    // docker run --name redis -d -p 6379:6379 redis --requirepass redispass
    using namespace sw::redis;
    ConnectionOptions connection_options;

    connection_options.host = REDIS_HOST;
    connection_options.port = REDIS_PORT;

    // 设置密码
    connection_options.password = REDIS_PASS;

    auto client = Redis(connection_options);
    client.set("name", "lsm1998");
    auto val = client.get("name");
    std::cout << "redis存储的值=" <<  val.value() << std::endl;
    return 0;
}