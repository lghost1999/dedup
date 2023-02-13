aux_source_directory(. SRC_LIST)
aux_source_directory(../shared SHARED_LIST)
aux_source_directory(../../proto PROTO_LIST)

add_executable(client ${SRC_LIST} ${SHARED_LIST} ${PROTO_LIST})

target_link_libraries(client ${RPC_LIB} m xxhash)
