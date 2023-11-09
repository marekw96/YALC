message("Raspberry pico-w build")

include_directories(${YALC_PROTOCOL_INCLUDES})
include_directories("system/raspberry_pico_w")

pico_sdk_init()

list(APPEND "RPI_PICO_W_SRC" "system/raspberry_pico_w/main.cpp")
list(APPEND "RPI_PICO_W_SRC" "system/raspberry_pico_w/Time.cpp")

add_executable(RpiPicoW_YALC ${YALC_PROTOCOL_SRC}  ${RPI_PICO_W_SRC})
target_link_libraries(RpiPicoW_YALC pico_stdlib)
pico_add_extra_outputs(RpiPicoW_YALC)