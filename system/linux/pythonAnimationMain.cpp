#include <iostream>

#include <fstream>
#include <string>

#include "Common.hpp"
#include "Time.hpp"
#include "../pythonAnimations/pocketpy.hpp"
#include "SFMLDisplay.hpp"

using namespace pkpy;

std::string readWholeFile(std::string fileName) {
    std::ifstream t(fileName);
    std::string str((std::istreambuf_iterator<char>(t)),
                 std::istreambuf_iterator<char>());

    return str;
}

SFMLDisplay* dsp;

void initVM(pkpy::VM* vm) {
    vm->bind(vm->_main, "__YALC__get_number_of_pixels() -> int", [](VM* vm, ArgsView args){
        return py_var(vm, 10);
    });

    vm->bind(vm->_main, "__YALC__set_pixel_color_rgb(id, r, g, b)", [](VM* vm, ArgsView args){
        byte data[] = {py_cast<byte>(vm, args[1]), py_cast<byte>(vm, args[2]), py_cast<byte>(vm, args[3])};
        auto id = py_cast<uint32_t>(vm, args[0]);
        dsp->setColors(id, data, 1);
        return vm->None;
    });

    vm->exec(readWholeFile("../pythonAnimations/YALCAnimation.py"));
    vm->exec(readWholeFile("../pythonAnimations/MyAnimation.py"));
    vm->exec("currentAnimation = create()");
}

int main() {
    Time time;
    SFMLDisplay sfmlDisplay(10, 350, 150);
    dsp = &sfmlDisplay;
    VM* vm = new VM();

    initVM(vm);

    auto current = time.current();
    while(sfmlDisplay.isOk()) {
        sfmlDisplay.periodic();
        auto diffUs = time.current() - current;
        vm->exec(std::string("currentAnimation.periodic(" + std::to_string(diffUs.asMiliseconds()) + ")"));

        current = time.current();
    }


    delete vm;
    return 0;
}