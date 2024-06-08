#pragma once

#include <string>
#include <map>
#include <vector>
#include <regex>
#include <arpa/inet.h>

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

class HttpResponse
{
private:
    int code;

public:
    int statusCode() const;
};