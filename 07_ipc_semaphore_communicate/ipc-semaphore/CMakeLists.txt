add_executable(ipc-semaphore semaphore.c)
set(CMAKE_C_STANDARD 11)
target_link_libraries(ipc-semaphore PRIVATE pthread)

