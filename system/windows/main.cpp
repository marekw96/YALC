#include <iostream>

#include "Time.hpp"
#include "Socket.hpp"

#include "DisplayController.hpp"
#include "DisplayNodeController.hpp"

int main() {
    Time time;
    Socket socket;
    DisplayController display;

    YALC::DisplayNodeController<Time, Socket, Socket, DisplayController> node(time, socket, socket, display);

    while(true)
        node.periodic();

    return 0;
}