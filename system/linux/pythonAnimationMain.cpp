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

int main(int argc, char* argv[]) {
    if(argc != 2){
        std::cerr << "Pass animation file name" << std::endl;
        return 1;
    }

    Time time;
    SFMLDisplay sfmlDisplay(25, 650, 150);

    auto* vm = new AnimationEngine(sfmlDisplay);

    vm->init();
    vm->createAnimation(readWholeFile(argv[1]));
    auto moduleName = vm->getModuleName();
    std::cout << "Module name is: " << moduleName << std::endl;
    auto moduleParameters = vm->getParameters();
    std::cout << "Module parameters[" << moduleParameters.size() << "]:" << std::endl;
    for(const auto& parameter: moduleParameters) {
        std::cout << "Paramter name: " << parameter.name << ", defaultValue: " << parameter.defaultValue << std::endl;
    }

    //vm->setColorParameterValue("fillColor", "aeaeae");

    auto current = time.current();

    while(sfmlDisplay.isOk()) {
        sfmlDisplay.periodic();
        auto diff = time.current() - current;
        vm->periodic(diff);
        current = time.current();
    }

    delete vm;

    return 0;
}