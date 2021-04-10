#pragma once

/**
 * INT 11H:
 * Determines the optional devices on the system and returns the value at location 40: 10H from the BIOS to the AX. (When turning on the equipment, the system executes this operation and stores the AX at location 40: 10H).
 * Bitfields for BIOS equipment list:
 * Bit (s) Description (Table 00226)
 * 0 floppy disk (s) installed (number specified by bits 7-6)
 * 1 80x87 coprocessor installed
 * 3-2 number of 16K banks of RAM on motherboard (PC only) .
 * Number of 64K banks of RAM on motherboard (XT only)
 * 2 pointing device installed (PS)
 * 3 unused (PS)
 * 5-4 initial video mode.
 *	00 EGA, VGA, or PGA.
 *	01 40x25 color.
 *  10 80x25 color.
 *	11 80x25 monochrome
 * 7-6 number of floppies installed less 1 (if bit 0 set)
 * 8 DMA support installed (PCjr, Tandy 1400LT).
 * DMA support * not * installed (Tandy 1000's)
 * 11-9 number of serial ports installed
 * 12 game port installed
 * 13 serial printer attached (PCjr).
 * Internal modem installed (PC / Convertible)
 * 15-14 number of parallel ports installed --- Compaq, Dell, and many other 386/486 machines--
 * 23 page tables set so that Weitek coprocessor addressable in real mode
 * 24 Weitek math coprocessor present
 * - --Compaq Systempro ---
 * 25 internal DMA parallel port available
 * 26 IRQ for internal DMA parallel port (if bit 25 set).
 *	0 = IRQ5.
 *	1 = IRQ7
 * 28-27 parallel port DMA channel.
 *	00 DMA channel 0.
 *	01 DMA channel 0 ???.
 * 10 reserved.
 * 11 DMA channel 3.
 */
namespace dos {

	/**
	 * Equipment flags.
	 F|E|D|C|B|A|9|8|7|6|5|4|3|2|1|0|  AX
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
	 */
	enum equipment_flags_t {
		DISKETTE_BOOT		= 0x1,
		MATH_COPROCESSOR	= 0x2,
		DMA_INSTALLED		= 0x100,
		GAME_ADAPTER		= 0x800
	};

	/**
	 * bits 5 & 4,  initial video mode
	 * 00 - EGA,VGA,PGA, or other with on-board video BIOS
	 * 01 - 40x25 color
	 * 10 - 80x25 color
	 * 11 - 80x25 monochrome
	 */
	enum initial_video_mode_t {
		OTHER = 0,
		INIT_COLOUR_40X25,
		INIT_COLOUR_80x25,
		INIT_MONO_80x25
	};

	static char init_mode_strings[4][255] = {
		"EGA,VGA,PGA, or other with on-board video BIOS",
		"40x25 colour",
		"80x25 colour",
		"80x25 monochrome"
	};

}
