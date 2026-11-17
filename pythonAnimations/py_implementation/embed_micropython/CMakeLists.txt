cmake_minimum_required(VERSION 3.16)

set(EMBED_DIR "${CMAKE_CURRENT_SOURCE_DIR}/micropython_embed")
set(GEN_MARKER "${EMBED_DIR}/genhdr/qstrdefs.generated.h")

find_program(MAKE_EXE NAMES make gmake mingw32-make REQUIRED)

# 1. Automatically generate micropython_embed during configure phase if it does not exist yet
if(NOT EXISTS "${GEN_MARKER}")
    message(STATUS "Generating MicroPython embed package at configure time...")
    execute_process(
        COMMAND ${MAKE_EXE} -f embed.mk USER_C_MODULES=modules CFLAGS_EXTRA=-DMODULE_LEDS_ENABLED=1
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        RESULT_VARIABLE MAKE_RES
    )
    if(NOT MAKE_RES EQUAL 0)
        message(FATAL_ERROR "Failed to generate micropython_embed package (exit code: ${MAKE_RES})")
    endif()
endif()

# 2. Rule to update micropython_embed package during build time
add_custom_command(
    OUTPUT "${GEN_MARKER}"
    COMMAND ${MAKE_EXE} -f embed.mk USER_C_MODULES=modules CFLAGS_EXTRA=-DMODULE_LEDS_ENABLED=1
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/embed.mk"
        "${CMAKE_CURRENT_SOURCE_DIR}/mpconfigport.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/modules/leds/leds.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/modules/leds/micropython.mk"
    COMMENT "Updating MicroPython embed package..."
)
add_custom_target(generate_micropython_embed DEPENDS "${GEN_MARKER}")

# 3. Collect MicroPython C sources
file(GLOB MP_PY_SRCS "${EMBED_DIR}/py/*.c")
file(GLOB MP_PORT_SRCS "${EMBED_DIR}/port/*.c")
file(GLOB MP_SHARED_SRCS "${EMBED_DIR}/shared/runtime/*.c")

set(MICROPYTHON_SOURCES
    ${MP_PY_SRCS}
    ${MP_PORT_SRCS}
    ${MP_SHARED_SRCS}
    "${CMAKE_CURRENT_SOURCE_DIR}/modules/leds/leds.c"
)

# 4. Static library MicroPythonAsLibrary
add_library(MicroPythonAsLibrary STATIC ${MICROPYTHON_SOURCES})
add_library(micropython_embed ALIAS MicroPythonAsLibrary)
add_dependencies(MicroPythonAsLibrary generate_micropython_embed)

target_compile_features(MicroPythonAsLibrary PRIVATE c_std_99)

target_include_directories(MicroPythonAsLibrary PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${EMBED_DIR}"
    "${EMBED_DIR}/port"
)

# 5. Link with pico_base in case of Pico / RP2040 architecture
if(TARGET pico_base)
    target_link_libraries(MicroPythonAsLibrary PRIVATE pico_base)
endif()
