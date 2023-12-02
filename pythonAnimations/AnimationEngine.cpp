extern "C" {

    void* display_obj;
    void* python_engine_obj;
    void (*func_to_set_pixel)(int id, int r, int g, int b);
    int (*func_to_get_numer_of_pixels)(void);
    void (*func_get_module_name)(const char* str);
    void (*func_register_color_parameter)(const char* name, const char* def);

    void set_pixel_id_r_g_b(int id , int r , int g, int b){
        func_to_set_pixel(id, r, g, b);
    }

    int get_number_of_pixels() {
        return func_to_get_numer_of_pixels();
    }
    void set_module_name(const char* str) {
        func_get_module_name(str);
    }
    void register_color_parameter(const char* name, const char* defaultValue) {
        func_register_color_parameter(name, defaultValue);
    }
}