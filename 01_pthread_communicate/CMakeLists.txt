add_executable(01_pthread_communicate process.c)
set(CMAKE_C_STANDARD 11)
target_link_libraries(01_pthread_communicate PRIVATE pthread)

