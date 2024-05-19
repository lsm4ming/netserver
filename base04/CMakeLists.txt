find_package(httplib REQUIRED)

add_executable(base04-httptest-bin httptest.cpp)

target_link_libraries(base04-httptest-bin httplib::httplib)