# 用于指定需要的CMamke的最低版本
cmake_minimum_required(VERSION 3.5)

# 用于指定项目名称，名字可以和项目根目录名字不同
project (hello_cmake)


# find_package(OpenCV REQUIRED)

# Add OpenCV headers location to your include paths
# include_directories(${OpenCV_INCLUDE_DIRS})

# 用于创建可执行文件，第一个参数是可执行文件名字，后面的参数是用于执行需要用到的源码文件
add_executable(hello_cmake bmp.h bmp.cpp)

# target_link_libraries(hello_cmake ${OpenCV_LIBS})