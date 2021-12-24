# 用于指定需要的CMamke的最低版本
cmake_minimum_required(VERSION 3.5)

# 用于指定项目名称，名字可以和项目根目录名字不同
project (bmp)


set(OpenCV_DIR "D:/opencv/build/x64/")

find_package(OpenCV REQUIRED)

# find_package(OpenCV REQUIRED)

# Add OpenCV headers location to your include paths
include_directories(${OpenCV_INCLUDE_DIRS})


# add_executable(bmp bmp.h bmputil.h bmputil.cpp bmpdemo_1.cpp)
add_executable(bmp bmp.h bmputil.h bmputil.cpp bmpdemo_2.cpp)
# add_executable(bmp bmp.h bmputil.h bmputil.cpp bmpdemo_3.cpp)
# add_executable(bmp bmp_24.cpp)

target_link_libraries(bmp ${OpenCV_LIBS})