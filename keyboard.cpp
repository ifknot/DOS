#include "keyboard.h"

namespace xt {

    keyboard& keyboard::instance() {
        static keyboard k;
        return k;
    }

    char keyboard::command(char cmd) {
        unsigned char result = 1;
        __asm {
                .8086
                PUSH AX
                PUSH DX
                PUSH CX
        KBD1:   IN AL, STATUS_PORT
                TEST AL, INPT_BUF_FULL
                LOOPNZ KBD1
                MOV AL, cmd
                OUT KBD_CMD_BUF, AL
                SUB CX, CX
        KBD2:   IN AL, STATUS_PORT
                TEST AL, OUT_BUF_FULL
                LOOPZ KBD2
                IN AL, KBD_OUT_BUF
                MOV result, AL
                POP CX
                POP DX
                POP AX
        }
        return result;
    }

    std::pair<char, char> keyboard::await_keypres() {
        return make_pair('A','B');
    }

}