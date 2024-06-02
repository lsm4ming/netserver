#include <iostream>
#include <httplib/httplib.h>
#include <curl/curl.h>

// 回调函数用于处理接收到的数据
size_t write_callback(void *ptr, size_t size, size_t nmemb, void *userdata)
{
    size_t written = fwrite(ptr, size, nmemb, stdout);
    return written;
}

void curlSend(const std::string &url)
{
    CURL *curl;
    CURLcode res;

    // 初始化curl会话
    curl = curl_easy_init();
    if (!curl)
    {
        perror("初始化失败");
        return;
    }

    // 设置要访问的URL
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    // 设置回调函数来写入接收到的数据
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);

    // 执行HTTP请求
    res = curl_easy_perform(curl);
    if (res != CURLE_OK)
    {
        fprintf(stderr, "curl执行失败: %s\n", curl_easy_strerror(res));
        return;
    }

    // 清理资源
    curl_easy_cleanup(curl);
}

int main()
{
    curlSend("http://www.baidu.com");

    std::cout << "-------------------------" << std::endl;

    httplib::Client client("www.baidu.com", 80);

    auto result = client.Get("/");
    auto resp = result.value();
    std::cout << resp.body << std::endl;
    return 0;
}