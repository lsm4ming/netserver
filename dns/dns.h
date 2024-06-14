#pragma once

#include <string>
#include <iostream>

constexpr const int DNS_SERVER_PORT = 53;
constexpr const char* DNS_SERVER_IP = "114.114.114.114";

constexpr const char DNS_HOST = 0x01;
constexpr const char DNS_CNAME = 0x05;

using String = std::string;

struct dns_header
{
	unsigned short id;
	unsigned short flags;

	unsigned short questions; // 1
	unsigned short answer;

	unsigned short authority;
	unsigned short additional;
};

struct dns_question
{
	int length;
	unsigned short qtype;
	unsigned short qclass;
	unsigned char *name; //
};

struct dns_item
{
	char *domain;
	char *ip;
};

class Dns
{
private:
    String domain;

public:
    Dns(const String &domain = "") : domain(domain){};
    ~Dns() = default;
};