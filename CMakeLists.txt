cmake_minimum_required(VERSION 3.9)

set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

project(process-communication)

add_subdirectory(00_fork_process)
add_subdirectory(01_pthread_communicate)
add_subdirectory(02_anonymous_pipeline_communicate)
add_subdirectory(03_filename_pipeline_communicate)
add_subdirectory(04_signal_communicate)
add_subdirectory(05_ipc_share_mem_communicate)
add_subdirectory(06_ipc_queue_communicate)
add_subdirectory(07_ipc_semaphore_communicate)


