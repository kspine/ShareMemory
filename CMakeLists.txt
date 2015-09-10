cmake_minimum_required(VERSION 3.3)
project(ShareMemory)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp src/ShareMemory.cpp src/ShareMemory.h)
add_executable(ShareMemory ${SOURCE_FILES})