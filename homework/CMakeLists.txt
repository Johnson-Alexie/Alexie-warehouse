
cmake_minimum_required(VERSION 3.18)


project(cmake_basic_demo)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_executable(hello_cmake c++learn.cpp)


