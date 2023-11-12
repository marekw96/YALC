#pragma once

#include "../pythonAnimations/pocketpy.hpp"
#include "../pythonAnimations/YALCAnimation.py.hpp"
#include "Time.hpp"

using namespace pkpy;

template <typename Display>
class AnimationEngine: public VM {
    public:
        AnimationEngine(Display& display)
            : VM(), display(display)
        {}

        void init(){
            bindGetNumberOfPixels();
            bindSetPixelColorRGB();
            execYALCAnimationBaseClass();
        }

        void periodic(TimeDuration diff) {
            vm->exec(std::string("currentAnimation.periodic(" + std::to_string(diff.asMicroseconds()) + ")"));
        }

    private:

        void bindGetNumberOfPixels() {
            this->bind(this->_main, "__YALC__get_number_of_pixels() -> int", [](VM* vmBase, ArgsView args){
                auto* vm = static_cast<AnimationEngine*>(vmBase);
                return py_var(vm, vm->display.ledsCount());
            });
        }

        void bindSetPixelColorRGB() {
            if(display.colorOrder() == YALC::ColorOrder::GRB) {
                this->bind(this->_main, "__YALC__set_pixel_color_rgb(id, r, g, b)", [](VM* vmBase, ArgsView args){
                    auto* vm = static_cast<AnimationEngine*>(vmBase);
                    byte data[] = {py_cast<byte>(vm, args[2]),
                                   py_cast<byte>(vm, args[1]),
                                   py_cast<byte>(vm, args[3])};
                    auto id = py_cast<uint32_t>(vm, args[0]);
                    vm->display.setColors(id, data, 1);
                    return vm->None;
                });
            }
            else { //RGB
                this->bind(this->_main, "__YALC__set_pixel_color_rgb(id, r, g, b)", [](VM* vmBase, ArgsView args){
                    auto* vm = static_cast<AnimationEngine*>(vmBase);
                    byte data[] = {py_cast<byte>(vm, args[1]),
                                   py_cast<byte>(vm, args[2]),
                                   py_cast<byte>(vm, args[3])};
                    auto id = py_cast<uint32_t>(vm, args[0]);
                    vm->display.setColors(id, data, 1);
                    return vm->None;
                });
            }
        }

        void execYALCAnimationBaseClass(){
            this->exec(YALCAnimation_py);
        }

        Display& display;
};
