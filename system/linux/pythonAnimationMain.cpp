#include <iostream>

#include <fstream>
#include <string>

#include "Common.hpp"
#include "Time.hpp"
#include "../pythonAnimations/AnimationEngine.hpp"
#include "../pythonAnimations/MyAnimation.py.hpp"
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
    std::cout << "sizeof(*vm) " << sizeof(*vm) << std::endl;
    vm->init();
    vm->createAnimation(MyAnimation_py);

    auto current = time.current();

    // for(int i = 0; i < 1000; ++i) {
    //     std::cout << "Loop: " << i << std::endl;
    //     sfmlDisplay.periodic();
    //     auto diff = time.current() - current;
    //     vm->periodic(diff);
    //     current = time.current();
    // }
    while(sfmlDisplay.isOk()) {
        sfmlDisplay.periodic();
        auto diff = time.current() - current;
        vm->periodic(diff);
        current = time.current();
    }


    delete vm;
    return 0;
}