// Include required definitions first.
#include "py/obj.h"
#include "py/runtime.h"
#include "py/builtin.h"

#include <stdio.h>

extern void set_pixel_id_r_g_b(int, int, int, int);
extern int get_number_of_pixels();
extern void set_module_name(const char* str);

// This is the function which will be called from Python as example.add_ints(a, b).
STATIC mp_obj_t leds_set_pixel_id_r_g_b(size_t n_args, const mp_obj_t *args) {
    // Extract the ints from the micropython input objects
    int id = mp_obj_get_int(args[0]);
    int r = mp_obj_get_int(args[1]);
    int g = mp_obj_get_int(args[2]);
    int b = mp_obj_get_int(args[3]);

    set_pixel_id_r_g_b(id, r,g,b);

    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(leds_set_pixel_id_r_g_b_obj, 4, 4, leds_set_pixel_id_r_g_b);


STATIC mp_obj_t leds_get_number_of_pixels() {
    return mp_obj_new_int(get_number_of_pixels());
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(leds_get_number_of_pixels_obj, leds_get_number_of_pixels);

STATIC mp_obj_t leds_set_module_name(mp_obj_t name) {
    const char* name_str = mp_obj_str_get_str(name);
    set_module_name(name_str);
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(leds_set_module_name_obj, leds_set_module_name);

// Define all properties of the example module.
// Table entries are key/value pairs of the attribute name (a string)
// and the MicroPython object reference.
// All identifiers and strings are written as MP_QSTR_xxx and will be
// optimized to word-sized integers by the build system (interned strings).
STATIC const mp_rom_map_elem_t leds_module_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_leds) },
    { MP_ROM_QSTR(MP_QSTR_set_pixel_id_r_g_b), MP_ROM_PTR(&leds_set_pixel_id_r_g_b_obj) },
    { MP_ROM_QSTR(MP_QSTR_get_number_of_pixels), MP_ROM_PTR(&leds_get_number_of_pixels_obj) },
    { MP_ROM_QSTR(MP_QSTR_set_module_name), MP_ROM_PTR(&leds_set_module_name_obj) },
};
STATIC MP_DEFINE_CONST_DICT(leds_module_globals, leds_module_globals_table);

// Define module object.
const mp_obj_module_t leds_user_cmodule = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t*)&leds_module_globals,
};


// Register the module to make it available in Python.
MP_REGISTER_MODULE(MP_QSTR_leds, leds_user_cmodule);