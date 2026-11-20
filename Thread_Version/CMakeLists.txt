cmake_minimum_required(VERSION 4.3)
project(Essay C)

set(CMAKE_C_STANDARD 11)

include_directories(..)

add_executable(Thread
        main.c
        p1.c
        p2.c
        p2.h
        semaphore.c
        semaphore.h
        p1.h)
#taskset -c 0 ./cmake-build-debug/Thread