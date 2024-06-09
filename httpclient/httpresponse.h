#pragma once

#include <string>
#include <map>
#include <vector>
#include <regex>
#include <arpa/inet.h>
#include <sstream>
#include <unistd.h>

using String = std::string;
using OsStringStream = std::ostringstream;
template <typename T>
using Vector = std::vector<T>;
using Header = std::map<String, Vector<String>>;

constexpr const int BUFFER_SIZE = 4096;
constexpr const char *HTTP_VERSION = "HTTP/1.1";
constexpr const char *WRAP = "\r\n";
constexpr const char *EMPTY = " ";
constexpr const char *GET_METHOD = "GET";

extern std::regex url_regex;
extern std::regex resp_line_regex;

class HttpRequest;

class HttpResponse
{
    friend HttpRequest;

private:
    int code{};
    String status;
    String body;
    size_t bodyLength{};
    Header header;

public:
    int statusCode() const;

    String httpStatus() const;

    String getBody() const;

private:
    void readResponse(int fd);
};