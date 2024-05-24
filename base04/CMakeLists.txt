find_package(httplib REQUIRED)

add_executable(base04-http_server-bin http_server.cpp)

target_link_libraries(base04-http_server-bin httplib::httplib)

add_executable(base04-http_client-bin http_client.cpp)

target_link_libraries(base04-http_client-bin httplib::httplib)

find_package(jsoncons REQUIRED)

add_executable(base04-json-bin json.cpp)

target_link_libraries(base04-json-bin jsoncons)

add_executable(base04-mysql-bin mysql_client.cpp)

add_executable(base04-redis-bin redis_client.cpp)

add_executable(base04-mongo-bin mongo_client.cpp)

find_package(yaml-cpp REQUIRED)

add_executable(base04-yaml-bin yaml.cpp)

target_link_libraries(base04-yaml-bin yaml-cpp::yaml-cpp)