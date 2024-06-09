#pragma once

#include "httpresponse.h"
#include "httprequest.h"

class HttpClient
{
private:
    HttpRequest request;

public:
    explicit HttpClient() = default;
    ~HttpClient() = default;
    HttpClient *setQuery(const String &key, const String &value);
    HttpClient *addQuery(const String &key, const String &value);
    HttpClient *setQuery(const String &key, const Vector<String> &values);
    HttpClient *addQuery(const String &key, const Vector<String> &values);
    HttpClient *setHeader(const String &key, const String &value);
    HttpClient *addHeader(const String &key, const String &value);
    HttpClient *setHeader(const String &key, const Vector<String> &values);
    HttpClient *addHeader(const String &key, const Vector<String> &values);
    HttpClient *setBody(char *body, size_t length);
    HttpClient *setTimeout(int seconds);
    HttpResponse post(const String &url);
    HttpResponse get(const String &url);
    HttpResponse doSend(const String &method, const String &url);
    HttpResponse doSend(const HttpRequest &request);
};