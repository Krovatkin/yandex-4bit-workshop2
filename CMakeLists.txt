cmake_minimum_required(VERSION 3.10)
project(gtest_param)

add_subdirectory(googletest)
add_library(number INTERFACE)
target_link_libraries(number INTERFACE ${CMAKE_SOURCE_DIR})

add_executable(my_test my_test.cpp)
set_property(TARGET my_test PROPERTY CXX_STANDARD 17)

target_link_libraries(my_test gtest gtest_main number)
