cmake_minimum_required(VERSION 2.8)

project(Test_FCD)

set(CMAKE_CXX_FLAGS_RELEASE)
find_package(OpenCV REQUIRED)
add_executable(TestFCD test_main.cpp FCD.cpp DOwnScale.cpp lsd.c)
target_link_libraries(TestFCD ${OpenCV_LIBS})
