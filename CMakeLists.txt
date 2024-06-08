cmake_minimum_required(VERSION 3.22)

project(netserver)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

set(CMAKE_CXX_COMPILER_VERSION 23)
set(CMAKE_CXX_STANDARD 23)

#message(STATUS "CMAKE_BINARY_DIR: ${CMAKE_BINARY_DIR}")
#include(${CMAKE_BINARY_DIR}/conan_toolchain.cmake)

add_subdirectory(base01)
add_subdirectory(base02)
add_subdirectory(base03)
add_subdirectory(base04)
add_subdirectory(daemon)
add_subdirectory(reactor-net)
add_subdirectory(tests)
add_subdirectory(httpclient)