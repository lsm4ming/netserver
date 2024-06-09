#include "httpclient.h"

HttpClient *HttpClient::setQuery(const String &key, const String &value)
{
    this->request.setQuery(key, value);
    return this;
}

HttpClient *HttpClient::addQuery(const String &key, const String &value)
{
    this->request.addQuery(key, value);
    return this;
}

HttpClient *HttpClient::setQuery(const String &key, const Vector<String> &values)
{
    this->request.setQuery(key, values);
    return this;
}

HttpClient *HttpClient::addQuery(const String &key, const Vector<String> &values)
{
    this->request.addQuery(key, values);
    return this;
}

HttpClient *HttpClient::setHeader(const String &key, const String &value)
{
    return this->setHeader(key, Vector<String>{value});
}

HttpClient *HttpClient::addHeader(const String &key, const String &value)
{
    return this->addHeader(key, Vector<String>{value});
}

HttpClient *HttpClient::setHeader(const String &key, const Vector<String> &values)
{
    this->request.setHeader(key, values);
    return this;
}

HttpClient *HttpClient::addHeader(const String &key, const Vector<String> &values)
{
    this->request.addHeader(key, values);
    return this;
}

HttpClient *HttpClient::setBody(char *body, size_t length)
{
    this->request.setBody(body, length);
    return this;
}

HttpClient *HttpClient::setTimeout(int seconds)
{
    this->request.setTimeout(seconds);
    return this;
}

HttpResponse HttpClient::post(const String &url)
{
    return this->doSend("POST", url);
}

HttpResponse HttpClient::get(const String &url)
{
    return this->doSend("GET", url);
}

HttpResponse HttpClient::doSend(const String &method, const String &url)
{
    return this->request.doSend(method, url);
}

HttpResponse HttpClient::doSend(const HttpRequest &request)
{
    this->request = request;
    return this->doSend(request.method, request.rawUrl);
}