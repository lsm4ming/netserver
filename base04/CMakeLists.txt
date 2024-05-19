find_package(httplib REQUIRED)

add_executable(base04-http_server-bin http_server.cpp)

target_link_libraries(base04-http_server-bin httplib::httplib)

add_executable(base04-http_client-bin http_client.cpp)

target_link_libraries(base04-http_client-bin httplib::httplib)