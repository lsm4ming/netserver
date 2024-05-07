project(netserver)

cmake_minimum_required(VERSION 3.22)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

add_subdirectory(base01)
add_subdirectory(base02)
add_subdirectory(base03)