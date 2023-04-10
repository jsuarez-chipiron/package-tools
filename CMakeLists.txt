cmake_minimum_required(VERSION 3.1)

project(package-tools)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")

add_executable(merge-packages merge-packages.cpp)
add_executable(extract-tests extract-tests.cpp)
