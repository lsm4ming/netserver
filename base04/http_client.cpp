#include <iostream>
#include <httplib.h>

int main()
{
    httplib::Client client("www.baidu.com" , 80);

    auto result = client.Get("/");
    auto resp = result.value();
    std::cout << resp.body << std::endl;
    return 0;
}