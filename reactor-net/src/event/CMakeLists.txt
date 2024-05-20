file(GLOB_RECURSE ALL_SOURCE "*.cpp" "*.c")

add_library(eventlib STATIC ${ALL_SOURCE})