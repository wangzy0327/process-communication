add_executable(server server.c)
add_executable(client client.c)
set(CMAKE_C_STANDARD 11)
target_link_libraries(server)
target_link_libraries(client)

