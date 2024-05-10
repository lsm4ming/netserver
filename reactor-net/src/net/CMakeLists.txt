file(GLOB_RECURSE ALL_SOURCE "*.cpp" "*.c")

add_library(netlib STATIC ${ALL_SOURCE})