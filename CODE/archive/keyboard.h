#pragma once

#include <iostream>
#include <utility>

/*
* 8042 Keyboard Controller
*/
#define STATUS_PORT     0x64
#define KBD_OUT_BUF     0x60
#define KBD_INPT_BUF    0x60
#define KBD_CMD_BUF     0x64
#define KBD_DATA_BUF    0x60
#define PORT_A          0x60
#define PORT_B          0x61
#define OUT_BUF_FULL    0x01
#define INPT_BUF_FULL   0x02
#define SYS_FLAG        0x04
#define CMD_DATA        0x08
#define KEYBD_INH       0x10
#define TRANS_TMOUT     0x20
#define RCV_TMOUT       0x40
#define PARITY_EVEN     0x80
#define INH_KEYBOARD    0x10
#define KBD_ENA         0xAE
#define KBD_DIS         0xAD
/*
* Keyboard Commands
*/
#define KB_READ_CMD     0x20
#define KB_WRITE_CMD    0x60
#define KB_IS_PWD       0xA4
#define KB_LOAD_PWD     0xA5
#define KB_ENABLE_PWD   0xA6
#define KB_DISABLE_AUX  0xA7
#define KB_ENABLE_AUX   0xA8
#define KB_TEST_AUX     0xA9
#define KB_SELF_TEST    0xAA
#define KB_INTERFACE    0xAB
#define KB_DUMP         0xAC
#define KB_DISABLE      0xAD
#define KB_ENABLE       0xAE
/*
* Keyboard responses
*/
#define KB_YES_PWD      0xFA
#define KB_NO_PWD       0xF1
#define KB_AUX_OK       0x00
/*
* INT 16 - Keyboard BIOS Services
*/
#define KB_READ         0x00
#define KB_KEY_STATUS   0x01
#define KB_SHIFT_STATUS 0x02
/*
* keyboard scan codes
*/
#define J_KEY           0x24
/*
* keyboard shift code
*/
#define RIGHT_SHIFT_KEY 0x01
#define LEFT_SHIFT_KEY  0x02
#define CTRL_KEY        0x04
#define ALT_KEY         0x08
#define SCROLL_LOCK     0x10
#define NUM_LOCK        0x20
#define CAPS_LOCK       0x40
#define INSERT          0x80


namespace dos {

    /**
    * C98 Singleton pattern
    */
    class keyboard {

    public:

        typedef std::pair<char, char> key_t;

        static keyboard& instance();

        char command(char cmd);

        key_t await_keypress();

        bool is_pressed(char scan_code);

        bool is_modifier(char flags);

    private:

        keyboard() {}
        keyboard(keyboard const&);
        void operator=(keyboard const&);

    };

}

//#endif  // KEYBOARD_H



