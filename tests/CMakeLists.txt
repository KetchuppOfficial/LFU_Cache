add_executable(lfu ./src/lfu.cpp)
add_executable(lfu_naive ./src/lfu.cpp)
add_executable(belady ./src/belady.cpp)
add_executable(belady_naive ./src/belady.cpp)

target_compile_definitions(lfu_naive
                           PRIVATE NAIVE)
target_compile_definitions(belady_naive
                           PRIVATE NAIVE)

target_include_directories(lfu
                           PRIVATE ${INCLUDE_DIR})
target_include_directories(lfu_naive
                           PRIVATE ${INCLUDE_DIR})

target_include_directories(belady
                           PRIVATE ${INCLUDE_DIR})
target_include_directories(belady_naive
                           PRIVATE ${INCLUDE_DIR})

install(TARGETS lfu
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        COMPONENT lfu)

install(TARGETS lfu_naive
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        COMPONENT lfu_naive)

install(TARGETS belady
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        COMPONENT belady)

install(TARGETS belady_naive
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        COMPONENT belady_naive)
