add_executable(share-mem-client share-mem-client.c)
add_executable(share-mem-server share-mem-server.c)
set(CMAKE_C_STANDARD 11)
target_link_libraries(share-mem-client)
target_link_libraries(share-mem-server)

