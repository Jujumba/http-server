cmake_minimum_required(VERSION 3.22)
set(CMAKE_C_STANDARD 23)

project(http-server C)
AUX_SOURCE_DIRECTORY(src SOURCES)
add_executable(http-server ${SOURCES})