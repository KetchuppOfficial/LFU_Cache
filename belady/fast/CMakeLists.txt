cmake_minimum_required(VERSION 3.16)

project(belady
        LANGUAGES CXX)

set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_STANDARD 20)

aux_source_directory(./src SRC_LIST)

add_executable(belady ${SRC_LIST})

target_include_directories(belady
                           PRIVATE ../include)
