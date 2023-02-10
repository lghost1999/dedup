cmake_minimum_required(VERSION 3.0)
project(test)

aux_source_directory(. SRC_LIST)
add_executable(test ${SRC_LIST})

target_link_libraries(test m)
