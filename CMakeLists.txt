cmake_minimum_required(VERSION 3.6)
project(AES)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp AES.cpp AES.h)
add_executable(AES ${SOURCE_FILES})