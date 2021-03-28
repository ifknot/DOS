#include <iostream>
#include <dos.h>

#include "keyboard.h"

int main() {



    xt::keyboard& k = xt::keyboard::instance();
    std::cout << "kbd self test\n";
    std::cout << std::hex << (int)k.command(KB_SELF_TEST) << '\n';
    std::cout << "kbd interface\n";
    std::cout << std::hex << (int)k.command(KB_INTERFACE) << '\n';
     std::cout << "kbd command\n";
    std::cout << std::hex << (int)k.command(KB_READ_CMD) << '\n';
    std::cout << "press j\n";
    while(!k.is_pressed(J_KEY)) {}
    std::cout << "pressed j\n";
    std::cout << "press a key...\n";
    xt::keyboard::key_t key = k.await_keypress();
    std::cout << std::hex << (int)key.first << ' ' << key.second << '\n';
    std::cout << k.is_pressed(J_KEY) << '\n';
    std::cout << k.is_modifier(CAPS_LOCK | INSERT) << '\n';

    std::cout << "OK\n";
    return 0;

}
