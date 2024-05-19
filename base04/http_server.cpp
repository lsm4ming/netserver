#include <iostream>
#include <httplib.h>

void hello(const httplib::Request &req, httplib::Response &resp)
{
    auto name =req.get_param_value("name");
    resp.body = "hello:" + name;
    // resp.status = 200;
}

int main()
{
    httplib::Server server;
    server.Get("/hello", hello);
    server.bind_to_port("127.0.0.1", 9999);
    server.listen_after_bind();
    return 0;
}