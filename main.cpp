#include <iostream>
#include <dos.h>

#include "keyboard.h"

int main() {

    std::cout << "Hello World!\n";

    xt::keyboard& k = xt::keyboard::instance();
    //std::cout << std::hex << (int)k.command(KB_SELF_TEST) << '\n';
    //std::cout << std::hex << (int)k.command(KB_INTERFACE) << '\n';
    //std::cout << std::hex << (int)k.command(KB_READ_CMD) << '\n';
    while(!k.is_pressed(0x24)) {
        //delay(500);
    }
    std::cout << "pressed j\n";
    std::cout << "press a key...\n";
    xt::keyboard::key_t key = k.await_keypress();
    std::cout << std::hex << (int)key.first << ' ' << key.second << '\n';
    std::cout << k.is_pressed(0x24) << '\n';
    std::cout << k.is_pressed(0x24) << '\n';

    std::cout << "OK\n";
    return 0;

}
