message("Raspberry pico-w build")

include_directories(${YALC_PROTOCOL_INCLUDES})
include_directories("system/raspberry_pico_w")

pico_sdk_init()

list(APPEND "RPI_PICO_W_SRC" "system/raspberry_pico_w/main.cpp")
list(APPEND "RPI_PICO_W_SRC" "system/raspberry_pico_w/Time.cpp")
list(APPEND "RPI_PICO_W_SRC" "system/raspberry_pico_w/ws2812/ws2812.cpp")
list(APPEND "RPI_PICO_W_SRC" "system/raspberry_pico_w/ws2812/LedString_ws2812.cpp")
list(APPEND "RPI_PICO_W_SRC" "system/raspberry_pico_w/Socket.cpp")


add_executable(RpiPicoW_YALC ${YALC_PROTOCOL_SRC}  ${RPI_PICO_W_SRC})
target_link_libraries(RpiPicoW_YALC pico_stdlib hardware_pio hardware_dma pico_cyw43_arch_lwip_poll)
pico_add_extra_outputs(RpiPicoW_YALC)
pico_generate_pio_header(RpiPicoW_YALC ${CMAKE_CURRENT_LIST_DIR}/ws2812/ws2812.pio)
