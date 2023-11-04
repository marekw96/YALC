cmake_minimum_required(VERSION 3.16)
project(YALC)

include("protocol/CMakeLists.txt")

message(STATUS "Declared target: ${TARGET}")

if( ${TARGET} STREQUAL "windows" )
    include("system/windows/CMakeLists.txt")
endif()

if( ${TARGET} STREQUAL "linux" )
    include("system/linux/CMakeLists.txt")
endif()