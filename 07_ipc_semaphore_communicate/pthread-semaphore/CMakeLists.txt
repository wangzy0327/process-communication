add_executable(pthread-semaphore thread.c)
set(CMAKE_C_STANDARD 11)
target_link_libraries(pthread-semaphore PRIVATE pthread)

