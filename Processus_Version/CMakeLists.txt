cmake_minimum_required(VERSION 4.3)
project(Processus C)
#taskset -c 0 ./
set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(Processus
        main.c
        p1.c
        p2.c
        p2.h
        semaphore.c
        semaphore.h
        p1.h)