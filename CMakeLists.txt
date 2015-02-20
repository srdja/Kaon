cmake_minimum_required(VERSION 3.0)

PROJECT(GAME C)

SET(ARCH "32" CACHE STRING "32")
#SET(MINGWLIB "" CACHE STRING "C:/MinGW/lib")

message(STATUS "Building for " ${ARCH} "bit architecture...")
#message(STATUS "Path is " ${MINGWLIB})

add_subdirectory(src)
