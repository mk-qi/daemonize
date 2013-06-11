cmake_minimum_required(VERSION 2.6)

# project name
project(daemonize C)

# show all warnings
add_definitions(-Wall)

# create shared library
add_library(daemonize
            SHARED 
            lib/src/daemonize.c)


install(FILES
        lib/src/daemonize.h
        DESTINATION
        ${CMAKE_CURRENT_SOURCE_DIR}/build/include)