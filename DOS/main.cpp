#include <iostream>
#include <iomanip>

//#include "test_keyboard.h"
//#include "test_equipment.h"
//#include "test_display.h"

/**
 * @brief Detect the presence of the Intel 8087 Numeric Processor Extension (NPX).
 * @note NB 1 The fwait opcode 9Bh occurs in all but 2 of the NPX instructions
 * fwait stops the CPu from procesing anymore instructions until its TEST pin is active and,
 * since this pin is connected to the BUSY pin of the NPX, the CPU will not send the NPX any 
 * instructions until it has finished with witht he previous one.
 * @note NB 2 The assembler automatically inserts the fwait opcode unless it is prevented 
 * from doing so by the presence of 'n' for - no wait - in the instruction mnemonic.
 * @return bool - true if NPX detect otherwise false
 */
bool detect_8087() {
    unsigned short status = 0xFF;
    __asm {
        .8086
        .8087
        push cx
        fninit          ; no wait initialise the NPX(if there is one)
        mov cx, 64h     ; idle in an empty loop whilst fninit loads the status word 
    L1: loop L1
        fnstsw status   ; save the status word which will reset the lower byte to 0 
        mov cx, 64h     ; idle in an empty loop whilst fnstsw stores the status word
    L2: loop L2
        pop cx
    }
    return status == 0;
}

int main() {

    std::cout << "*** test harness ***\n\n";

    //assert(test_keyboard());
    //assert(test_equipment());
    //assert(test_display());
    
    std::cout << "detect npx = " << (detect_8087() ?"yes" :"no") << '\n';

    std::cout << std::hex;
    std::cout << std::setiosflags(std::ios::showbase | std::ios::uppercase);
    std::cout << 100 << std::endl;
   
    std::cout << "OK\n";
    return 0;

}
