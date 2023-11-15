cmake_minimum_required(VERSION 3.16)
project(PyText)
add_subdirectory(py_implementation/embed_micropython)



set (CMAKE_CXX_STANDARD 17)

add_executable(PyText main.cpp)
target_include_directories(PyText PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          "${PROJECT_SOURCE_DIR}/py_implementation/embed_micropython/micropython_embed"
                          )
target_link_libraries(PyText MicroPythonAsLibrary)