aux_source_directory(./src SRC_LIST)

add_executable(lfu_naive ${SRC_LIST})

target_include_directories(lfu_naive
                           PRIVATE ../include)

install(TARGETS lfu_naive
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        COMPONENT lfu_naive)
