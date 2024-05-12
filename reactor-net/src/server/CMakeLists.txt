file(GLOB_RECURSE ALL_SOURCE "*.cpp" "*.c")

add_library(serverlib STATIC ${ALL_SOURCE})

target_link_libraries(serverlib PUBLIC netlib)
