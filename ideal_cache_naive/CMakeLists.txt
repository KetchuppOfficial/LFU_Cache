cmake_minimum_required(VERSION 3.16)

project(Ideal_Cache_Naive
        LANGUAGES CXX)

set(CMAKE_CXX_COMPILER clang++)

aux_source_directory(./src SRC_LIST)

add_executable(Ideal_Cache_Naive ${SRC_LIST})

target_include_directories(Ideal_Cache_Naive
                           PRIVATE ../include)
