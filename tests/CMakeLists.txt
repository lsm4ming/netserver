add_executable(tests-test-bin test.cpp demo.cpp)

find_package(GTest REQUIRED)
target_link_libraries(tests-test-bin gtest::gtest)