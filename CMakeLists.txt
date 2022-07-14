cmake_minimum_required(VERSION 3.9)

set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

project(process-communication)

add_subdirectory(00_fork_process)
add_subdirectory(01_pthread_communicate)


