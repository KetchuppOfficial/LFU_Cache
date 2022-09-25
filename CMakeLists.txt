cmake_minimum_required(VERSION 3.16)

project(HWC
        LANGUAGES CXX)

if (${PROJECT_SOURCE_DIR} STREQUAL ${PROJECT_BINARY_DIR})
    message(FATAL_ERROR "In-source build is forbidden")
endif()

set(CMAKE_CXX_STANDARD          20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        OFF)

set(ALGORITHM)
set(IMPLEMENTATION)

if (DEFINED ALGORITHM)
    if (${ALGORITHM} STREQUAL lfu)
        add_subdirectory(lfu)
    elseif (${ALGORITHM} STREQUAL belady)
        add_subdirectory(belady)
    else()
        message(FATAL_ERROR "Subdirectory \"${ALGORITHM}\" is not a part of the project")
    endif()
else()
    add_subdirectory(lfu)
    add_subdirectory(belady)
endif()
