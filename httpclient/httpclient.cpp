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

        this->query = this->parseQuery(this->queryRaw);
        this->send();
    }
    else
    {
        std::cout << "The URL is not valid." << std::endl;
    }
    return this;
}

String HttpClient::host_to_ip(String hostname)
{
    auto *host_entry = gethostbyname(hostname.c_str()); // dns
    if (host_entry)
    {
        return inet_ntoa(*(struct in_addr *)*host_entry->h_addr_list);
    }
    return "";
}

int HttpClient::http_create_socket(String ip)
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
    fcntl(sockfd, F_SETFL, O_NONBLOCK);
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

void HttpClient::send()
{
    // 组装请求url
    this->rawUrl = assembleUrl();
    std::cout << "请求url=" << this->rawUrl << std ::endl;
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

int main()
{
    HttpClient client;
    auto a = client.get("http://www.baidu.com");
    return 0;
}