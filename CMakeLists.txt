cmake_minimum_required(VERSION 3.16)

project(LFU_Cache
        LANGUAGES CXX)

set(CMAKE_CXX_COMPILER clang++)

set(SRC_LIST
    lfu.cpp)

add_executable(LFU_Cache ${SRC_LIST})
