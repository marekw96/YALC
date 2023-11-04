message("Linux build!")

message("included ${YALC_PROTOCOL_INCLUDES}")
message("src ${YALC_PROTOCOL_SRC}")

include_directories(${YALC_PROTOCOL_INCLUDES})
include_directories("system/linux")

find_package(SFML 2.5 COMPONENTS graphics REQUIRED)

list(APPEND "LINUX_SRC" "system/linux/Time.cpp")
list(APPEND "LINUX_SRC" "system/linux/Socket.cpp")
list(APPEND "LINUX_SRC" "system/linux/SFMLDisplay.cpp")

add_executable(LinuxMain system/linux/main.cpp ${YALC_PROTOCOL_SRC} ${LINUX_SRC})
target_link_libraries(LinuxMain PRIVATE sfml-graphics)