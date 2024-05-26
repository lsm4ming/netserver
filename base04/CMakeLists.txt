find_package(httplib REQUIRED)

add_executable(base04-http_server-bin http_server.cpp)

target_link_libraries(base04-http_server-bin httplib::httplib)

add_executable(base04-http_client-bin http_client.cpp)

target_link_libraries(base04-http_client-bin httplib::httplib)

find_package(jsoncons REQUIRED)

add_executable(base04-json-bin json.cpp)

target_link_libraries(base04-json-bin jsoncons)

find_package(redis++ REQUIRED)

add_executable(base04-redis-bin redis_client.cpp)

target_link_libraries(base04-redis-bin redis++::redis++_static)

add_executable(base04-mongo-bin mongo_client.cpp)

find_package(yaml-cpp REQUIRED)

add_executable(base04-yaml-bin yaml.cpp)

target_link_libraries(base04-yaml-bin yaml-cpp::yaml-cpp)

find_package(libmysqlclient REQUIRED)

add_executable(base04-mysql-bin mysql_client.cpp)

target_link_libraries(base04-mysql-bin libmysqlclient::libmysqlclient)

# 打印libmysqlclient库的包含目录
message(STATUS "libmysqlclient库目录: ${libmysqlclient_INCLUDE_DIRS}")