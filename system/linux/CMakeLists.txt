message("Linux build!")

message("included ${YALC_PROTOCOL_INCLUDES}")
message("src ${YALC_PROTOCOL_SRC}")

project(YALC C CXX)

set (CMAKE_CXX_STANDARD 17)

include("${PROJECT_SOURCE_DIR}/pythonAnimations/animations.cmake")
add_subdirectory(pythonAnimations/py_implementation/embed_micropython/)

include_directories(${YALC_PROTOCOL_INCLUDES})
include_directories("system/linux")

find_package(SFML 2.5 COMPONENTS graphics REQUIRED)

list(APPEND "LINUX_SRC" "system/linux/Time.cpp")
list(APPEND "LINUX_SRC" "system/linux/Socket.cpp")
list(APPEND "LINUX_SRC" "system/linux/SFMLDisplay.cpp")
list(APPEND "LINUX_SRC" "pythonAnimations/AnimationEngine.cpp")

add_executable(pythonAnimationMain "system/linux/pythonAnimationMain.cpp" ${LINUX_SRC})
add_dependencies(pythonAnimationMain generate_animation_headers)

target_include_directories(pythonAnimationMain PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          "${PROJECT_SOURCE_DIR}/pythonAnimations/py_implementation/embed_micropython/micropython_embed"
                          )
target_link_libraries(pythonAnimationMain PUBLIC sfml-graphics micropython_embed)