message("Windows build!")

message("included ${YALC_PROTOCOL_INCLUDES}")
message("src ${YALC_PROTOCOL_SRC}")

include_directories(${YALC_PROTOCOL_INCLUDES})
include_directories("system/windows")

list(APPEND "WINDOWS_SRC" "system/windows/Time.cpp")
list(APPEND "WINDOWS_SRC" "system/windows/Socket.cpp")

add_executable(WindowsMain system/windows/main.cpp ${YALC_PROTOCOL_SRC} ${WINDOWS_SRC})