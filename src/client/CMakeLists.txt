cmake_minimum_required(VERSION 3.0)
project(client)

include_directories(${PROJECT_SOURCE_DIR}/..)
include_directories(${PROJECT_SOURCE_DIR})

aux_source_directory(. SRC_LIST)
aux_source_directory(${PROJECT_SOURCE_DIR}/.. COMMON_LIST)
add_executable(client ${SRC_LIST} ${COMMON_LIST})

set(CMAKE_CXX_FLAGS "${CMAKE_CPP_FLAGS} -DNDEBUG -O2 -D__const__=__unused__ -pipe -W -Wall -Wno-unused-parameter -fPIC -fno-omit-frame-pointer")
target_link_libraries(client m xxhash)
