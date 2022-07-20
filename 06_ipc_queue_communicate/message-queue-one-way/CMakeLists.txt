add_executable(mq-client mq-client.c)
add_executable(mq-server mq-server.c)
set(CMAKE_C_STANDARD 11)
target_link_libraries(mq-client)
target_link_libraries(mq-server)

