#include "httpclient.h"
#include <iostream>

int main()
{
    HttpClient client;
    auto resp = client.setQuery("name", "lsm")->get("http://www.baidu.com");
    std::cout << "statusCode=" << resp.statusCode() << std::endl;
    std::cout << "httpStatus=" << resp.httpStatus() << std::endl;
    std::cout << "body=" << resp.getBody() << std::endl;


    HttpRequest request("http://www.baidu.com");
    resp = client.doSend(request);
    std::cout << "body=" << resp.getBody() << std::endl;
    return 0;
}