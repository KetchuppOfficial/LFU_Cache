cmake_minimum_required(VERSION 3.16)

project(HWC
        LANGUAGES CXX)

if (${PROJECT_SOURCE_DIR} STREQUAL ${PROJECT_BINARY_DIR})
    message(FATAL_ERROR "In-source build is forbidden")
endif()

set(CMAKE_CXX_STANDARD          20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        OFF)

set(CACHE_TYPE) # user-defined variable

if (${CACHE_TYPE} STREQUAL "lfu")
    add_subdirectory(lfu)
elseif (${CACHE_TYPE} STREQUAL "belady")
    add_subdirectory(belady)
else ()
    add_subdirectory(lfu)
    add_subdirectory(belady)
endif ()
