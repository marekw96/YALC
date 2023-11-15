cmake_minimum_required(VERSION 3.16)

include("protocol/CMakeLists.txt")

message(STATUS "Declared target: ${TARGET}")

if( ${TARGET} STREQUAL "windows" )
    include("system/windows/CMakeLists.txt")
endif()

if( ${TARGET} STREQUAL "linux" )
    include("system/linux/CMakeLists.txt")
endif()

if( ${TARGET} STREQUAL "raspberry_pico_w" )
    set(PICO_BOARD "pico_w")
    include("system/raspberry_pico_w/pico-sdk/pico_sdk_init.cmake")
    project(YALC CXX C ASM)


    include("system/raspberry_pico_w/CMakeLists.txt")
endif()