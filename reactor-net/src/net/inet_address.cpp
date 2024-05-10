#include "inet_address.h"

InetAddress::InetAddress()
{
}

InetAddress::InetAddress(const std::string &ip, uint16_t port)
{
    this->addr_.sin_family = AF_INET;
    this->addr_.sin_addr.s_addr = inet_addr(ip.c_str());
    this->addr_.sin_port = htons(port);
}

InetAddress::InetAddress(const sockaddr_in addr) : addr_(addr)
{
}

InetAddress::~InetAddress()
{
}

const char *InetAddress::ip() const
{
    return inet_ntoa(this->addr_.sin_addr);
}

uint16_t InetAddress::port() const
{
    return ntohs(this->addr_.sin_port);
}

const sockaddr *InetAddress::addr() const
{
    return (sockaddr *)&this->addr_;
}

void InetAddress::setaddr(sockaddr_in client_addr)
{
    this->addr_ = client_addr;
}