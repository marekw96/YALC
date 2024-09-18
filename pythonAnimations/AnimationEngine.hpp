#pragma once

extern "C" {
    #include "port/micropython_embed.h"

    extern void* display_obj;
    extern void* python_engine_obj;
    extern void (*func_to_set_pixel)(int id, int r, int g, int b);
    extern int (*func_to_get_numer_of_pixels)(void);
    extern void (*func_get_module_name)(const char* str);
    extern void (*func_register_color_parameter)(const char* name, const char* def);
    extern void (*func_register_input)(const char* name, int type);

    void set_pixel_id_r_g_b(int id , int r , int g, int b);
    int get_number_of_pixels();
    void set_module_name(const char* str);
    void register_color_parameter(const char* name, const char* defaultValue);
    void register_input(const char* name, int type);
}
#include "../pythonAnimations/YALCAnimation.py.hpp"
#include "Time.hpp"
#include <string>
#include <vector>
#include "../pythonAnimations/Parameter.hpp"

static char heap[32 * 1024];

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
            if(initDone) {
                mp_embed_deinit();
                initDone = false;
            }
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
            bindRegisterInput();
            execYALCAnimationBaseClass();

            initDone = true;
        }

        void changeAnimation(const std::string& code) {
            deinit();
            init();
            createAnimation(code);
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
            parameters.clear();
            this->exec("currentAnimation.registerParameters()");
            return parameters;
        }

        const std::vector<InputDescription> getInputs() {
            inputs.clear();
            this->exec("currentAnimation.registerInputs()");
            return inputs;
        }

        void setColorParameterValue(const std::string& name, const std::string& value) {
            std::string colorPython = std::string("[0x") + value.substr(0, 2) + ", 0x" + value.substr(2,2) + ", 0x" + value.substr(4,2) + "]";
            std::string cmd = std::string("currentAnimation.setParameter('") + name + "', " + colorPython + ")";
            this->exec(cmd);
        }

        void handleInput(const std::string& name, const std::string& value){
            std::string cmd = std::string("currentAnimation.handleInput('") + name + "', " + value + ")";
            //printf("handleInput %s\n", cmd.c_str());
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

        void _addInput(const char* name, int type) {
            InputDescription input;

            if(type == static_cast<int>(InputType::RAW))
                input.type = InputType::RAW;
            else if(type == static_cast<int>(InputType::DIGITAL))
                input.type = InputType::DIGITAL;
            else if(type == static_cast<int>(InputType::ANALOG))
                input.type = InputType::ANALOG;
            else
                input.type = InputType::INVALID;

            input.name = name;
            inputs.push_back(input);
            printf("[VM]%s %d\n", name, type);
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
            func_to_set_pixel = [](int id, int r, int g, int b) -> void {
                Display* display = reinterpret_cast<Display*>(display_obj);
                display->setPixel(id, r,g,b);
            };
        }

        void bindRegisterInput() {
            func_register_input = [](const char* name, int type) -> void {
                AnimationEngine<Display>* ae = reinterpret_cast<AnimationEngine<Display>*>(python_engine_obj);
                ae->_addInput(name, type);
            };
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
        std::vector<InputDescription> inputs;
};
