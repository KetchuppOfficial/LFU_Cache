aux_source_directory(./src SRC_LIST)

add_executable(belady ${SRC_LIST})

target_include_directories(belady
                           PRIVATE ../include)

install(TARGETS belady
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        COMPONENT belady)
