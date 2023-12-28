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
    bool shouldEnd = false;
    std::string params = "";
    uint32_t ledStringLength = 100;
    uint32_t wrapLeds = 25;
    uint32_t windowWidth = 650;

    if(argc <= 1){
        std::cerr << "Pass animation file name" << std::endl;
        return 1;
    }

    for(int i = 1; i < argc; ++i){
        std::string arg = argv[i];

        if(arg == "oneLoop")
            shouldEnd = true;
        else if(arg == "--params"){
            if(i+1 < argc){
                params = argv[i+1];
                std::cout << "Got some parameters:" << params << std::endl;
            }
        }
        else if(arg == "--length") {
            if(i+1 < argc){
                ledStringLength = std::atoi(argv[i+1]);
                std::cout << "Got length:" << ledStringLength << std::endl;
            }
        }
        else if(arg == "--windowWidth") {
            if(i+1 < argc){
                windowWidth = std::atoi(argv[i+1]);
                std::cout << "Got window width:" << windowWidth << std::endl;
            }
        }
        else if(arg == "--wrapLeds") {
            if(i+1 < argc){
                wrapLeds = std::atoi(argv[i+1]);
                std::cout << "Got wrap leds:" << wrapLeds << std::endl;
            }
        }

    }


    Time time;
    SFMLDisplay sfmlDisplay(ledStringLength, windowWidth, 150, wrapLeds);

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

    if(!params.empty()){
        size_t pos = 0;
        while(true){
            std::string name;
            std::string value;

            auto nend = params.find("=", pos);


            if(nend != std::string::npos) {
                name = params.substr(0, nend);
            }else{
                break;
            }
            auto vend = params.find(";", nend);
            if(vend != std::string::npos) {
                value = params.substr(nend+1, vend-nend);
            }
            else{
                break;
            }

            std::cout << "Apply parameter: name: " << name << ", value: " << value << std::endl;
            vm->setColorParameterValue(name, value);

            pos = vend+1;
        }
    }

    //vm->setColorParameterValue("fillColor", "aeaeae");

    auto current = time.current();

    while(sfmlDisplay.isOk()) {
        sfmlDisplay.periodic();
        auto diff = time.current() - current;
        vm->periodic(diff);
        current = time.current();

        if(shouldEnd)
            break;
    }

    delete vm;

    return 0;
}