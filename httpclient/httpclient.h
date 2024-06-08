#pragma once

#include <fcntl.h>
#include <netdb.h>
#include <unistd.h>
#include <sstream>
#include "httpresponse.h"

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
    char *body{};
    size_t bodyLength{};
    int socketFd{};

public:
    HttpClient(/* args */) = default;
    ~HttpClient();
    HttpClient *setQuery(const String &key, const String &value);
    HttpClient *addQuery(const String &key, const String &value);
    HttpClient *setQuery(const String &key, const Vector<String> &values);
    HttpClient *addQuery(const String &key, const Vector<String> &values);
    HttpClient *setHeader(const String &key, const String &value);
    HttpClient *addHeader(const String &key, const String &value);
    HttpClient *setHeader(const String &key, const Vector<String> &values);
    HttpClient *addHeader(const String &key, const Vector<String> &values);
    HttpClient *setBody(char *body, size_t bodyLength);
    HttpClient *setTimeout(int seconds);
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