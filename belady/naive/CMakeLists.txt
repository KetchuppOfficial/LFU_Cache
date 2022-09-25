aux_source_directory(./src SRC_LIST)

add_executable(belady_naive ${SRC_LIST})

target_include_directories(belady_naive
                           PRIVATE ../include)

install(TARGETS belady_naive
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        COMPONENT belady_naive)
