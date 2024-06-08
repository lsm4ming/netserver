#pragma once

#include <string>
#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <map>
#include <vector>
#include <regex>
#include <unistd.h>

using String = std::string;
using OsStringStream = std::ostringstream;
template <typename T>
using Vector = std::vector<T>;
using Header = std::map<String, Vector<String>>;

constexpr const int BUFFER_SIZE = 4096;
constexpr const char *HTTP_VERSION = "HTTP/1.1";
constexpr const char *CONNETION_TYPE = "Connection: close\r\n";
constexpr const char *WRAP = "\r\n";
constexpr const char *EMPTY = " ";

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
    char *body;
    size_t bodyLength;

public:
    HttpClient(/* args */) = default;
    ~HttpClient() = default;
    HttpClient *setQuery(const String &key, const String &value);
    HttpClient *addQuery(const String &key, const String &value);
    HttpClient *setBody(char *body, size_t bodyLength);
    HttpClient *post(const String &url);
    HttpClient *get(const String &url);
    HttpClient *doSend(const String &method, const String &url);

private:
    static String host_to_ip(const String &hostname);

    static String joinValues(const Vector<String> &values);

    static void set_socket_timeout(int fd, int seconds);

    int http_create_socket(const String &ip);

    Header parseQuery(const String &queryRaw);

    void send();

    String assembleUrl();

    String encodeQueryParameters();
};