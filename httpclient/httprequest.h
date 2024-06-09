#pragma once

#include <netdb.h>
#include "httpresponse.h"

class HttpClient;

class HttpRequest
{
    friend HttpClient;

private:
    // 协议
    String protocol;
    String domain;
    String path;
    String queryRaw;
    String rawUrl;
    String method{GET_METHOD};
    Header header;
    Header query;
    char *body{};
    size_t length{};
    int socketFd{};

public:
    HttpRequest() = default;
    HttpRequest(String method, String url) : method(method), rawUrl(url) {}
    HttpRequest(String url) : rawUrl(url) {}
    ~HttpRequest() = default;

    void setQuery(const String &key, const String &value);
    void addQuery(const String &key, const String &value);
    void setQuery(const String &key, const Vector<String> &values);
    void addQuery(const String &key, const Vector<String> &values);
    void setHeader(const String &key, const String &value);
    void addHeader(const String &key, const String &value);
    void setHeader(const String &key, const Vector<String> &values);
    void addHeader(const String &key, const Vector<String> &values);
    void setBody(char *body, size_t length);
    void setTimeout(int seconds);
    HttpResponse post(const String &url);
    HttpResponse get(const String &url);
    HttpResponse doSend(const String &method, const String &url);

private:
    static String host_to_ip(const String &hostname);

    static String joinValues(const Vector<String> &values);

    static void set_socket_timeout(int fd, int seconds);

    String assembleUrl();

    String encodeQueryParameters();

    int http_create_socket(const String &ip);

    Header parseQuery(const String &queryRaw);

    HttpResponse send();
};
