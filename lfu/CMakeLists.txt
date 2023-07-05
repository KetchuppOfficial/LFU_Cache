add_executable(lfu ./src/lfu.cpp)
add_executable(lfu_naive ./src/lfu_naive.cpp)

target_include_directories(lfu
                           PRIVATE ./include)

target_include_directories(lfu_naive
                           PRIVATE ./include)

install(TARGETS lfu
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        COMPONENT lfu)

install(TARGETS lfu_naive
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        COMPONENT lfu_naive)
