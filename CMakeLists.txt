cmake_minimum_required(VERSION 3.18)
project(opencv_cmake_demo)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 核心命令：自动查找系统中安装的OpenCV库
find_package(OpenCV REQUIRED)

# 打印查找到的OpenCV版本，方便调试确认
message(STATUS "OpenCV 版本: " ${OpenCV_VERSION})

# 引入OpenCV头文件路径
include_directories(${OpenCV_INCLUDE_DIRS})

# 生成可执行文件
add_executable(opencv_demo opencv1.cpp)

# 把OpenCV库链接到生成的可执行文件上
target_link_libraries(opencv_demo ${OpenCV_LIBS})
