#pragma once

#include <string>
#include <unistd.h>
#include <vector>
#include <list>

// 创建socket fd失败
constexpr const int ERROR_CREATE_SOCKET_FD = -1001;

using String = std::string;

using StringView = std::string_view;

template <typename T>
using List = std::list<T>;

template <typename T>
using Vector = std::vector<T>;

template<class K, class V>
using Tuple2 = std::tuple<K, V>;