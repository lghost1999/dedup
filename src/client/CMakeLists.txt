cmake_minimum_required(VERSION 3.0)
project(client)

include_directories(${PROJECT_SOURCE_DIR}/..)
include_directories(${PROJECT_SOURCE_DIR}/client)

aux_source_directory(. SRC_LIST)
aux_source_directory(${PROJECT_SOURCE_DIR}/.. COMMON_LIST)
add_executable(client ${SRC_LIST} ${COMMON_LIST})

target_link_libraries(client m xxhash)
