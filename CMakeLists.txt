cmake_minimum_required(VERSION 3.22)

project(netserver)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

set(CONAN_TOOLCHAIN_FILE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake-build-debug/conan_toolchain.cmake)
message(STATUS "Using Conan toolchain file: ${CONAN_TOOLCHAIN_FILE_PATH}")
set(CMAKE_TOOLCHAIN_FILE ${CONAN_TOOLCHAIN_FILE_PATH} CACHE FILEPATH "Conan toolchain file")

set(CMAKE_CXX_COMPILER_VERSION 23)

add_subdirectory(base01)
add_subdirectory(base02)
add_subdirectory(base03)
add_subdirectory(base04)
add_subdirectory(daemon)
add_subdirectory(reactor-net)