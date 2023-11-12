#include <iostream>

#include <fstream>
#include <string>

#include "Common.hpp"
#include "Time.hpp"
#include "../pythonAnimations/AnimationEngine.hpp"
#include "SFMLDisplay.hpp"

std::string readWholeFile(std::string fileName) {
    std::ifstream t(fileName);
    std::string str((std::istreambuf_iterator<char>(t)),
                 std::istreambuf_iterator<char>());

    return str;
}

int main() {
    Time time;
    SFMLDisplay sfmlDisplay(10, 350, 150);

    auto* vm = new AnimationEngine(sfmlDisplay);
    vm->init();
    vm->exec(readWholeFile("../pythonAnimations/MyAnimation.py"));
    vm->exec("currentAnimation = create()");

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