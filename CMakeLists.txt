cmake_minimum_required(VERSION 2.6)

# project name
project(daemonize)

# project include path
include_directories("src/include")

# create shared library
add_library(daemonize
            SHARED 
            src/daemonize.c)


install(FILES
        src/include/daemonize.h
        DESTINATION
        ${CMAKE_CURRENT_SOURCE_DIR}/build/include)