add_executable(mq-fork-client mq-fork-client.c)
add_executable(mq-fork-server mq-fork-server.c)
set(CMAKE_C_STANDARD 11)
target_link_libraries(mq-fork-client)
target_link_libraries(mq-fork-server)

