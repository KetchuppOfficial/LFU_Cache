aux_source_directory(./src SRC_LIST)

add_executable(lfu_naive ${SRC_LIST})

target_include_directories(lfu_naive
                           PRIVATE ../include)
