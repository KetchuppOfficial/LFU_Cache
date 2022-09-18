cmake_minimum_required(VERSION 3.16)

project(belady_naive
        LANGUAGES CXX)

set(CMAKE_CXX_COMPILER clang++)

aux_source_directory(./src SRC_LIST)

add_executable(belady_naive ${SRC_LIST})

target_include_directories(belady_naive
                           PRIVATE ../include)
