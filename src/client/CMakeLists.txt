aux_source_directory(. SRC_LIST)
aux_source_directory(../shared SHARED_LIST)

add_executable(client ${SRC_LIST} ${SHARED_LIST})

target_link_libraries(client ${RPC_RPC_LIB} m xxhash)
