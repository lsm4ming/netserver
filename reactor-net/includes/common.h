#pragma once

#include <string>
#include <unistd.h>
#include <vector>
#include <list>
#include <iostream>

constexpr const int OK = 1;
// 创建socket fd失败
constexpr const int ERROR_CREATE_SOCKET_FD = -1001;
// bind fd失败
constexpr const int ERROR_BIND_SOCKET = -1002;
// listen fd失败
constexpr const int ERROR_LISTEN_SOCKET = -1003;

using String = std::string;

using StringView = std::string_view;

template <typename T>
using List = std::list<T>;

template <typename T>
using Vector = std::vector<T>;

template<class K, class V>
using Tuple2 = std::tuple<K, V>;