include_directories(includes)

add_subdirectory(src)

add_executable(reactor-server-bin main.cpp)

target_link_libraries(reactor-server-bin pthread netlib serverlib)