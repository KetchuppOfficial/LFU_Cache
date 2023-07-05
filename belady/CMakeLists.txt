add_executable(belady ./src/belady.cpp)
add_executable(belady_naive ./src/belady_naive.cpp)

target_include_directories(belady
                           PRIVATE ./include)

target_include_directories(belady_naive
                           PRIVATE ./include)

install(TARGETS belady
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        COMPONENT belady)

install(TARGETS belady_naive
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        COMPONENT belady_naive)
