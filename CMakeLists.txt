# 1. 强制要求CMake最低版本，避免低版本语法不兼容
cmake_minimum_required(VERSION 3.18)

# 2. 声明项目名称，可自定义
project(cmake_basic_demo)

# 3. 指定C++标准为C++17，OpenCV开发通用版本
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 4. 生成可执行文件，第一个参数是生成的程序名，后面跟所有依赖的源码文件
add_executable(hello_cmake c++learn.cpp)
