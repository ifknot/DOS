#include <iostream>

#include "keyboard.h"

int main() {

    std::cout << "Hello World!\n";

    xt::keyboard& k = xt::keyboard::instance();
    std::cout << std::hex << (int)k.command(KB_SELF_TEST) << '\n';
    std::cout << std::hex << (int)k.command(KB_INTERFACE) << '\n';
    std::cout << std::hex << (int)k.command(KB_READ_CMD) << '\n';
    std::cout << "OK\n";
    return 0;

}
