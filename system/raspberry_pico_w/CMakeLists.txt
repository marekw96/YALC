message("Raspberry pico-w build")

add_subdirectory(pythonAnimations/py_implementation/embed_micropython/)

include_directories(${YALC_PROTOCOL_INCLUDES})
include_directories("system/raspberry_pico_w")
include_directories("discover")
set (CMAKE_CXX_STANDARD 17)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fexceptions")
set(PICO_CXX_ENABLE_EXCEPTIONS 1)
set(PK_ENABLE_OS 0)


pico_sdk_init()

add_subdirectory("system/raspberry_pico_w/littlefs-lib")

list(APPEND "RPI_PICO_W_SRC" "system/raspberry_pico_w/main.cpp")
list(APPEND "RPI_PICO_W_SRC" "system/raspberry_pico_w/Time.cpp")
list(APPEND "RPI_PICO_W_SRC" "system/raspberry_pico_w/ws2812/ws2812.cpp")
list(APPEND "RPI_PICO_W_SRC" "system/raspberry_pico_w/ws2812/LedString_ws2812.cpp")
list(APPEND "RPI_PICO_W_SRC" "system/raspberry_pico_w/Socket.cpp")
list(APPEND "RPI_PICO_W_SRC" "system/raspberry_pico_w/InternetManager.cpp")
list(APPEND "RPI_PICO_W_SRC" "system/raspberry_pico_w/WebServer.cpp")
list(APPEND "RPI_PICO_W_SRC" "system/raspberry_pico_w/webpages/ConnectionSettings.cpp")
list(APPEND "RPI_PICO_W_SRC" "system/raspberry_pico_w/webpages/EffectsPage.cpp")
list(APPEND "RPI_PICO_W_SRC" "system/raspberry_pico_w/webpages/IndexPage.cpp")
list(APPEND "RPI_PICO_W_SRC" "system/raspberry_pico_w/webpages/LedsConfigurationPage.cpp")
list(APPEND "RPI_PICO_W_SRC" "system/raspberry_pico_w/webpages/RebootPage.cpp")
list(APPEND "RPI_PICO_W_SRC" "system/raspberry_pico_w/EffectsManager.cpp")
list(APPEND "RPI_PICO_W_SRC" "system/raspberry_pico_w/Storage.cpp")
list(APPEND "RPI_PICO_W_SRC" "system/raspberry_pico_w/LedsConfiguration.cpp")
list(APPEND "RPI_PICO_W_SRC" "system/raspberry_pico_w/UDPSocket.cpp")
list(APPEND "RPI_PICO_W_SRC" "pythonAnimations/AnimationEngine.cpp")
list(APPEND "RPI_PICO_W_SRC" "discover/Discover.cpp")


add_executable(RpiPicoW_YALC ${YALC_PROTOCOL_SRC}  ${RPI_PICO_W_SRC})
target_include_directories(RpiPicoW_YALC PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          "${PROJECT_SOURCE_DIR}/pythonAnimations/py_implementation/embed_micropython/micropython_embed"
                          )
target_link_libraries(RpiPicoW_YALC pico_stdlib hardware_pio hardware_dma pico_cyw43_arch_lwip_poll MicroPythonAsLibrary pico_lwip_core pico_lwip_core4 littlefs-lib)
pico_add_extra_outputs(RpiPicoW_YALC)
pico_generate_pio_header(RpiPicoW_YALC ${CMAKE_CURRENT_LIST_DIR}/ws2812/ws2812.pio)
pico_enable_stdio_usb(RpiPicoW_YALC 1)
pico_enable_stdio_uart(RpiPicoW_YALC 0)