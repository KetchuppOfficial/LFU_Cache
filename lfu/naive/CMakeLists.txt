cmake_minimum_required(VERSION 3.16)

project(lfu_naive
        LANGUAGES CXX)

set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_STANDARD 20)

aux_source_directory(./src SRC_LIST)

add_executable(lfu_naive ${SRC_LIST})

target_include_directories(lfu_naive
                           PRIVATE ../include)
