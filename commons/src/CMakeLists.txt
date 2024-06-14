file(GLOB_RECURSE ALL_SOURCE "*.cpp" "*.c")

add_library(commonslib STATIC ${ALL_SOURCE})