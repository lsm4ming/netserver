#pragma once

#include <string>
#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <map>
#include <vector>
#include <regex>

using String = std::string;
using OsStringStream = std::ostringstream;
template <typename T>
using Vector = std::vector<T>;
using Header = std::map<String, Vector<String>>;

constexpr const int BUFFER_SIZE = 4096;
constexpr const char *HTTP_VERSION = "HTTP/1.1";
constexpr const char *CONNETION_TYPE = "Connection: close\r\n";

std::regex url_regex(R"((https?)://([^/\r\n]+)(/[^\r\n?]*)?(\?[^#\r\n]*)?)");

class HttpClient
{
private:
    // 协议
    String protocol;
    String domain;
    String path;
    String queryRaw;
    String rawUrl;
    String method;
    Header header;
    Header query;

public:
    HttpClient(/* args */) = default;
    ~HttpClient() = default;
    HttpClient *setQuery(const String &key, const String &value);
    HttpClient *addQuery(const String &key, const String &value);
    HttpClient *post(const String &url);
    HttpClient *get(const String &url);
    HttpClient *doSend(const String &method, const String &url);

private:
    static String host_to_ip(String hostname);

    int http_create_socket(String ip);

    Header parseQuery(const String& queryRaw);

    void send();

    String assembleUrl();

    String encodeQueryParameters();
};