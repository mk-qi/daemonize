cmake_minimum_required(VERSION 2.6)

# project name
project(daemon C)

add_definitions(-Wall)

find_library(${CMAKE_CURRENT_SOURCE_DIR}/../../../build/libdaemonize.so
             REQUIRED)

# library path
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../daemonize/build/)

# project include path
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../daemonize/build/include)

# binary
add_executable(daemon
               main.c
               log.c)

# use shared library
target_link_libraries(daemon daemonize)
