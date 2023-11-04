#include <iostream>

#include "Time.hpp"
#include "Socket.hpp"

#include "DisplayNodeController.hpp"

#include "SFMLDisplay.hpp"

int main() {
    Time time;
    Socket socket(5432);
    SFMLDisplay sfmlDisplay(10, 350, 150);

    YALC::DisplayNodeController<Time, Socket, Socket, SFMLDisplay> node(time, socket, socket, sfmlDisplay);

    while(sfmlDisplay.isOk()) {
        node.periodic();
        sfmlDisplay.periodic();
    }

    return 0;
}