cmake_minimum_required(VERSION 3.16)

project(LFU_Naive
        LANGUAGES CXX)

set(CMAKE_CXX_COMPILER clang++)

aux_source_directory(./src SRC_LIST)

add_executable(LFU_Naive ${SRC_LIST})

target_include_directories(LFU_Naive
                           PRIVATE ../include)
