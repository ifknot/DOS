#include "keyboard.h"
#include <dos.h>

#include <iostream>

namespace xt {

    keyboard& keyboard::instance() {
        static keyboard k;
        return k;
    }

    char keyboard::command(char cmd) {
        char result = 1;
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

    keyboard::key_t keyboard::await_keypress() {
        union REGS r;
        r.h.ah = KB_AWAIT_READ;
        int86(0x16, &r, &r);
        return std::make_pair(r.h.ah,r.h.al);
    }

    bool keyboard::is_pressed(char scan) {
        char pressed = 0;   // default false
        __asm {

                .8086
                push ax
                mov ah, KB_KEY_STATUS
                int 0x16
                je NO           // is a key pressed?
                mov ah, KB_AWAIT_READ
                int 0x16
                cmp ah, scan    // is it the right one?
                jne NO
                mov pressed, 1  // set true
        NO:     pop ax

        }
        return (bool)pressed;
    }

}