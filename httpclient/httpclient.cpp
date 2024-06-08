#include "httpclient.h"
#include <iostream>

HttpClient *HttpClient::setQuery(const String &key, const String &value)
{
    this->query[key] = Vector<String>{value};
    return this;
}

HttpClient *HttpClient::addQuery(const String &key, const String &value)
{
    // auto val = this->query.find(key);
    return this;
}

HttpClient *HttpClient::setBody(char *body, size_t bodyLength)
{
    this->body = body;
    this->bodyLength = bodyLength;
    return this;
}

HttpClient *HttpClient::post(const String &url)
{
    return this->doSend("POST", url);
}

HttpClient *HttpClient::get(const String &url)
{
    return this->doSend("GET", url);
}

HttpClient *HttpClient::doSend(const String &method, const String &url)
{
    this->method = method;
    this->rawUrl = url;

    std::smatch url_match_result;
    // 匹配URL
    if (std::regex_match(url, url_match_result, url_regex) && url_match_result.size() >= 5)
    {
        this->protocol = url_match_result[1].str();
        this->domain = url_match_result[2].str();
        this->path = url_match_result[3].str();
        this->queryRaw = url_match_result[4].str();

        if (this->path.empty())
        {
            this->path = "/";
        }

        this->query = this->parseQuery(this->queryRaw);
        this->send();
    }
    else
    {
        std::cout << "The URL is not valid." << std::endl;
    }
    return this;
}

String HttpClient::host_to_ip(const String &hostname)
{
    auto *he = gethostbyname(hostname.c_str()); // dns
    if (he == nullptr)
    {
        return "";
    }
    auto **addr_list = (struct in_addr **)he->h_addr_list;
    return inet_ntoa(*addr_list[0]);
}

int HttpClient::http_create_socket(const String &ip)
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in sin = {0};
    sin.sin_family = AF_INET;
    sin.sin_port = htons(80); //
    sin.sin_addr.s_addr = inet_addr(ip.c_str());
    if (0 != connect(sockfd, (struct sockaddr *)&sin, sizeof(struct sockaddr_in)))
    {
        return -1;
    }
    // fcntl(sockfd, F_SETFL, O_NONBLOCK);
    return sockfd;
}

Header HttpClient::parseQuery(const String &queryRaw)
{
    Header queryMap;
    std::string::size_type start = 0;
    std::string::size_type end = queryRaw.find('&');

    while (end != std::string::npos)
    {
        std::string pair = queryRaw.substr(start, end - start);
        std::string::size_type delimiter = pair.find('=');
        if (delimiter != std::string::npos)
        {
            std::string key = pair.substr(0, delimiter);
            std::string value = pair.substr(delimiter + 1);
            queryMap[key].push_back(value);
        }
        start = end + 1;
        end = queryRaw.find('&', start);
    }

    // 处理最后一对键值（或只有一对时）
    std::string pair = queryRaw.substr(start);
    std::string::size_type delimiter = pair.find('=');
    if (delimiter != std::string::npos)
    {
        std::string key = pair.substr(0, delimiter);
        std::string value = pair.substr(delimiter + 1);
        queryMap[key].push_back(value);
    }
    return queryMap;
}

String HttpClient::joinValues(const Vector<String> &values)
{
    OsStringStream result;
    for (auto it = values.begin(); it != values.end(); ++it)
    {
        if (it != values.begin())
        {
            result << ", ";
        }
        result << *it;
    }
    return result.str();
}

void HttpClient::send()
{
    // 组装请求url
    this->rawUrl = assembleUrl();
    std::cout << "请求url=" << this->rawUrl << std ::endl;

    // 将domain转为IP地址
    String ip = this->host_to_ip(this->domain);

    // 建立TCP连接
    int socketFd = this->http_create_socket(ip);
    // set_socket_timeout(socketFd, 10);

    OsStringStream stream;

    // 写请求行
    stream << method << EMPTY << path << EMPTY << HTTP_VERSION << WRAP;
    // 写请求头
    for (auto &item : this->header)
    {
        stream << item.first << ": " << joinValues(item.second) << WRAP;
    }
    stream << WRAP;
    write(socketFd, stream.str().c_str(), stream.str().length());
    // 写请求体
    if (this->body && this->bodyLength > 0)
    {
        write(socketFd, this->body, this->bodyLength);
    }

    // 读取回复
    OsStringStream responseStream;
    char buffer[BUFFER_SIZE];
    int bytesReceived;
    size_t contentLength = 0;

    // 读取响应头
    String headers;
    while ((bytesReceived = read(socketFd, buffer, 1)) > 0)
    {
        headers.append(buffer, 1);
        if (buffer[0] == '\n')
        {
            // 当读取到两个连续的换行符时表示找到了响应头的结束
            if (headers.substr(headers.length() - 4) == "\r\n\r\n")
            {
                break;
            }
        }
    }

    // 解析响应头
    std::istringstream headersStream(headers);
    std::string headerLine;
    while (std::getline(headersStream, headerLine) && headerLine != "\r")
    {
        if (headerLine.find("Content-Length:") != std::string::npos)
        {
            contentLength = std::stoi(headerLine.substr(16));
            break;
        }
    }
    responseStream << headers;

    // 读取响应体
    if (contentLength > 0)
    {
        std::cout << "contentLength=" << contentLength << std::endl;
        size_t remaining = contentLength;
        while (remaining > 0 && (bytesReceived = read(socketFd, buffer, std::min(remaining, size_t(BUFFER_SIZE)))) > 0)
        {
            std::cout << "contentLength读取一次" << std::endl;
            buffer[bytesReceived] = '\0';
            responseStream << buffer;
            remaining -= bytesReceived;
            std::cout << "remaining=" << remaining << ",bytesReceived=" << bytesReceived << std::endl;
        }
        if (bytesReceived < 0)
        {
            perror("recv");
            close(socketFd);
            exit(1);
        }
    }

    std::cout << "响应数据=" << responseStream.str() << std::endl;

    // 关闭连接
    close(socketFd);
}

String HttpClient::assembleUrl()
{
    OsStringStream url;
    url << protocol << "://" << domain;

    if (!path.empty())
    {
        if (path.front() != '/')
        {
            url << '/';
        }
        url << path;
    }
    String encodedQuery = encodeQueryParameters();
    if (!encodedQuery.empty())
    {
        url << '?' << encodedQuery;
    }

    return url.str();
}

String HttpClient::encodeQueryParameters()
{
    OsStringStream queryStream;
    for (auto it = query.begin(); it != query.end(); ++it)
    {
        for (const auto &value : it->second)
        {
            if (it != query.begin() || &value != &it->second.front())
            {
                queryStream << '&';
            }
            queryStream << it->first << '=' << value;
        }
    }
    return queryStream.str();
}

void HttpClient::set_socket_timeout(int fd, int seconds)
{
    struct timeval timeout;
    timeout.tv_sec = seconds;
    timeout.tv_usec = 0;
    setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, (const char *)&timeout, sizeof(timeout));
}

int main()
{
    HttpClient client;
    auto a = client.get("http://www.baidu.com");
    return 0;
}