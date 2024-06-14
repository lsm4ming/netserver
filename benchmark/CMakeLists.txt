add_executable(tcp-benchmark-bin tcp_benchmark.cpp)

get_filename_component(PARENT_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)

file(GLOB_RECURSE ALL_SOURCE "${PARENT_DIR}/commons/src/*.cpp")

add_library(commonslib STATIC ${ALL_SOURCE})

include_directories(${PARENT_DIR}/commons/includes)

target_link_libraries(tcp-benchmark-bin pthread commonslib)