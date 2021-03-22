#include <iostream>
#include <dos.h>
#include <stdint>

#define EVEN_LINES 0xB8000
#define ODD_LINES 0xBA000

unsigned char *mem[] = {(unsigned char*) EVEN_LINES,
                        (unsigned char*) ODD_LINES};

int read_key_stroke();
#pragma aux read_key_stroke = \
"   xor ax, ax"\
"   int 0x16"\
parm [ax]\
value [ax];

void pixel(uint16_t c);
#pragma aux pixel = \
"   mov ax, 0xB800 \\ even lines "\
"   mov es, ax"\
"   mov di, 0"\
"   mov ax,0AAh"\
"   rep stosb"\
parm [cx]\
modify [ax cx es di];


int main() {

    std::cout << "Hello World!\n";

    std::cout << "Switch mode?\n";

    union REGS r;

    // int 16h keyboard services
    /**
        INT 16,0   Wait for keystroke and read
        INT 16,1   Get keystroke status
        INT 16,2   Get shift status
        INT 16,3   Set keyboard typematic rate (AT+)
        INT 16,4   Keyboard click adjustment (AT+)
        INT 16,5   Keyboard buffer write  (AT,PS/2 enhanced keyboards)
        INT 16,10  Wait for keystroke and read  (AT,PS/2 enhanced keyboards)
        INT 16,11  Get keystroke status  (AT,PS/2 enhanced keyboards)
        INT 16,12  Get shift status  (AT,PS/2 enhanced keyboards)


        - with IBM BIOS's, INT 16 functions do not restore the flags to
          the pre-interrupt state to allow returning of information via
          the flags register
        - functions 3 through 12h are not available on all AT machines
          unless the extended keyboard BIOS is present
        - all registers are preserved except AX and FLAGS
    */

    read_key_stroke();

    // int 10h set video mode
    /**
        AH = 00
        AL = 00  40x25 B/W text (CGA,EGA,MCGA,VGA)
           = 01  40x25 16 color text (CGA,EGA,MCGA,VGA)
           = 02  80x25 16 shades of gray text (CGA,EGA,MCGA,VGA)
           = 03  80x25 16 color text (CGA,EGA,MCGA,VGA)
           = 04  320x200 4 color graphics (CGA,EGA,MCGA,VGA)
           = 05  320x200 4 color graphics (CGA,EGA,MCGA,VGA)
           = 06  640x200 B/W graphics (CGA,EGA,MCGA,VGA)
           = 07  80x25 Monochrome text (MDA,HERC,EGA,VGA)
           = 08  160x200 16 color graphics (PCjr)
           = 09  320x200 16 color graphics (PCjr)
           = 0A  640x200 4 color graphics (PCjr)
           = 0B  Reserved (EGA BIOS function 11)
           = 0C  Reserved (EGA BIOS function 11)
           = 0D  320x200 16 color graphics (EGA,VGA)
           = 0E  640x200 16 color graphics (EGA,VGA)
           = 0F  640x350 Monochrome graphics (EGA,VGA)
           = 10  640x350 16 color graphics (EGA or VGA with 128K)
                 640x350 4 color graphics (64K EGA)
           = 11  640x480 B/W graphics (MCGA,VGA)
           = 12  640x480 16 color graphics (VGA)
           = 13  320x200 256 color graphics (MCGA,VGA)
           = 8x  EGA
           , MCGA or VGA ignore bit 7, see below
           = 9x  EGA, MCGA or VGA ignore bit 7, see below


        - if AL bit 7=1, prevents EGA,MCGA & VGA from clearing display
        - function updates byte at 40:49;  bit 7 of byte 40:87
          (EGA/VGA Display Data Area) is set to the value of AL bit 7
    */

    r.h.ah = 0;
    r.h.al = 6;
    int86(0x10, &r, &r);

    pixel(320);

    r.x.ax = 0x0000;
    int86(0x16, &r, &r);

    std::cout << (uint16_t)r.h.ah << "\t" << r.h.al << '\n';

    // int 1A realtime clock bios service
    /**
        AH = 00 read system clock counter

        on return:
        AL = midnight flag, 1 if 24 hours passed since reset
        CX = high order word of tick count
        DX = low order word of tick count
    */
    r.x.ax = 0x0000;
    int86(0x1A, &r, &r);
    std::cout << r.x.cx << ' ' << r.x.dx << '\n';
    uint16_t sec = r.x.dx;

    delay(1000); // wait 1 second

    r.x.ax = 0x0000;
    int86(0x1A, &r, &r);
    std::cout << r.x.cx << ' ' << r.x.dx << '\n' << (r.x.dx - sec) / 18 << " seconds\n";

    std::cout << "\nSwitch back?\n";

    pixel(160);

    r.x.ax = 0x0000;
    int86(0x16, &r, &r);

    r.h.ah = 0;
    r.h.al = 2;
    int86(0x10, &r, &r);

     std::cout << (uint16_t)r.h.ah << "\t" << r.h.al;

}
