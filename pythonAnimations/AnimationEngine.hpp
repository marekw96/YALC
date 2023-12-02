#pragma once

extern "C" {
    #include "port/micropython_embed.h"

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
#include "../pythonAnimations/YALCAnimation.py.hpp"
#include "Time.hpp"
#include <string>
#include <vector>

static char heap[16 * 1024];

enum class ParameterType {
    Color = 'c'
};

struct ParameterDescription {
    ParameterType type;
    std::string name;
    std::string defaultValue;
    std::string value;
};

template <typename Display>
class AnimationEngine {
    public:
        AnimationEngine(Display& display)
            : display(display)
        {
            display_obj = &display;
            python_engine_obj = this;
        }

        ~AnimationEngine() {
            deinit();
        }

        void deinit() {
            mp_embed_deinit();
            initDone = false;
        }

        void init(){
            printf("[AnimationEngine]heap size %d\n", sizeof(heap));
            if(initDone) {
                return;
            }
            for(int i = 0; i < sizeof(heap); ++i) heap[i] = 0;

            mp_embed_init(&heap[0], sizeof(heap));

            bindGetNumberOfPixels();
            bindSetPixelColorRGB();
            bindGetModuleName();
            bindRegisterColorParameter();
            execYALCAnimationBaseClass();

            initDone = true;
        }

        void periodic(Duration diff) {
            this->exec(std::string("currentAnimation.periodic(" + std::to_string(diff.asMicroseconds()) + ")"));
        }

        void createAnimation(const std::string& code) {
            this->exec(code);
            this->exec("currentAnimation = create()");
        }

        const std::string getModuleName() {
            this->exec("leds.set_module_name(currentAnimation.name())");
            return moduleName;
        }

        const std::vector<ParameterDescription> getParameters() {
            this->exec("currentAnimation.registerParameters()");
            return parameters;
        }

        void setColorParameterValue(const std::string& name, const std::string& value) {
            std::string colorPython = std::string("[0x") + value.substr(0, 2) + ", 0x" + value.substr(2,2) + ", 0x" + value.substr(4,2) + "]";
            std::string cmd = std::string("currentAnimation.setParameter('") + name + "', " + colorPython + ")";
            std::cout << cmd << std::endl;
            this->exec(cmd);
        }

        void _setModuleName(const char* name) {
            moduleName = name;
        }

        void _addColorParameter(const char* name, const char* defaultValue) {
            ParameterDescription param;
            param.type = ParameterType::Color;
            param.name = name;
            param.defaultValue = defaultValue;
            param.value = defaultValue;
            parameters.push_back(param);
        }

    private:

        void bindGetNumberOfPixels() {
            func_to_get_numer_of_pixels = []() -> int {
                Display* display = reinterpret_cast<Display*>(display_obj);
                return static_cast<int>(display->ledsCount());
            };
        }

        void bindGetModuleName() {
            func_get_module_name = [](const char* name) -> void {
                AnimationEngine<Display>* ae = reinterpret_cast<AnimationEngine<Display>*>(python_engine_obj);
                ae->_setModuleName(name);
            };
        }

        void bindRegisterColorParameter() {
            func_register_color_parameter = [](const char* name, const char* defaultValue) -> void {
                AnimationEngine<Display>* ae = reinterpret_cast<AnimationEngine<Display>*>(python_engine_obj);
                ae->_addColorParameter(name, defaultValue);
            };
        }

        void bindSetPixelColorRGB() {
            if(display.colorOrder() == YALC::ColorOrder::GRB) {
                func_to_set_pixel = [](int id, int r, int g, int b) -> void {
                    Display* display = reinterpret_cast<Display*>(display_obj);
                    byte data[] = {(byte)g,(byte)r,(byte)b};
                    display->setColors(id, data, 1);
                };
            }
            else {
                func_to_set_pixel = [](int id, int r, int g, int b) -> void {
                    Display* display = reinterpret_cast<Display*>(display_obj);
                    byte data[] = {(byte)r,(byte)g,(byte)b};
                    display->setColors(id, data, 1);
                };
            }
        }

        void execYALCAnimationBaseClass(){
            this->exec(YALCAnimation_py);
        }

        void exec(const std::string& code) {
            mp_embed_exec_str(code.c_str());
        }


        Display& display;
        bool initDone = false;
        std::string moduleName;
        std::vector<ParameterDescription> parameters;
};
