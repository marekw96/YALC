#include "pocketpy.hpp"

using namespace pkpy;

int main(){
    // Create a virtual machine
    VM* vm = new VM();

    // Dispose the virtual machine
    delete vm;
    return 0;
}