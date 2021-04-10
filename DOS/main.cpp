#include <iostream>

#include "test_keyboard.h"
#include "test_equipment.h"
#include "test_display.h"

int main() {

    std::cout << "*** test harness ***\n\n";

    //assert(test_keyboard());
    assert(test_equipment());
    assert(test_display());
   
    std::cout << "OK\n";
    return 0;

}
