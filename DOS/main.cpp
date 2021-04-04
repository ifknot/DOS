#include <iostream>

#include "keyboard.h"

#include "test_keyboard.h"

int main() {

    std::cout << "*** test harness ***\n\n";

    assert(test_keyboard());

    

    std::cout << "OK\n";
    return 0;

}
