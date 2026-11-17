# Automatic generation of C++ headers (*.py.hpp) from Python scripts
find_package(Python3 REQUIRED COMPONENTS Interpreter)

set(PYTHON_ANIM_DIR "${PROJECT_SOURCE_DIR}/pythonAnimations")
set(ANIM_SCRIPTS
    "${PYTHON_ANIM_DIR}/YALCAnimation.py"
    "${PYTHON_ANIM_DIR}/animations/MyAnimation.py"
    "${PYTHON_ANIM_DIR}/animations/FallingStar.py"
)

set(GENERATED_ANIM_HPPS "")

foreach(SCRIPT_PATH IN LISTS ANIM_SCRIPTS)
    get_filename_component(SCRIPT_NAME "${SCRIPT_PATH}" NAME)
    set(OUT_HPP "${PYTHON_ANIM_DIR}/${SCRIPT_NAME}.hpp")
    
    add_custom_command(
        OUTPUT "${OUT_HPP}"
        COMMAND Python3::Interpreter "${PYTHON_ANIM_DIR}/convertToHpp.py" "${SCRIPT_PATH}" "${OUT_HPP}"
        DEPENDS "${SCRIPT_PATH}" "${PYTHON_ANIM_DIR}/convertToHpp.py"
        COMMENT "Generating ${SCRIPT_NAME}.hpp from ${SCRIPT_NAME}..."
    )
    list(APPEND GENERATED_ANIM_HPPS "${OUT_HPP}")
endforeach()

if(NOT TARGET generate_animation_headers)
    add_custom_target(generate_animation_headers DEPENDS ${GENERATED_ANIM_HPPS})
endif()
