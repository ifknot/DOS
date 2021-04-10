#pragma once

#include <stdint.h>

#include "equipment_types.h"

namespace dos {

    /**
    * C98 Singleton pattern
    * INT 11 - BIOS Equipment Determination / BIOS Equipment Flags

	no input data

	on return:
	AX contains the following bit flags:

	|F|E|D|C|B|A|9|8|7|6|5|4|3|2|1|0|  AX
	 | | | | | | | | | | | | | | | `---- IPL diskette installed
	 | | | | | | | | | | | | | | `----- math coprocessor
	 | | | | | | | | | | | | `-------- old PC system board RAM < 256K
	 | | | | | | | | | | | | | `----- pointing device installed (PS/2)
	 | | | | | | | | | | | | `------ not used on PS/2
	 | | | | | | | | | | `--------- initial video mode
	 | | | | | | | | `------------ # of diskette drives, less 1
	 | | | | | | | `------------- 0 if DMA installed
	 | | | | `------------------ number of serial ports
	 | | | `------------------- game adapter installed
	 | | `-------------------- unused, internal modem (PS/2)
	 `----------------------- number of printer ports


	- bits 3 & 2,  system board RAM if less than 256K motherboard
	    00 - 16K		     01 - 32K
	    10 - 16K		     11 - 64K (normal)

	- bits 5 & 4,  initial video mode
	    00 - unused 	     01 - 40x25 color
	    10 - 80x25 color	     11 - 80x25 monochrome


	- bits 7 & 6,  number of disk drives attached, when bit 0=1
	    00 - 1 drive	     01 - 2 drives
	    10 - 3 drive	     11 - 4 drives


	- returns data stored at BIOS Data Area location 40:10
	- some flags are not guaranteed to be correct on all machines
	- bit 13 is used on the PCjr to indicate serial printer
    */
    class equipment {

    public:

        static equipment& instance();

		bool detect(); // equipment flags

		bool is_floppy_boot();

		bool is_math_coprocessor();

		int boot_video_mode();

		int floppy_drive_count();

		int serial_device_count();

		bool is_game_port();

		int parallel_device_count();

		int memory_size();

		void info(); // device_info_struct

    private:

        equipment() {}
        equipment(equipment const&);
        void operator=(equipment const&) {}

    };

}