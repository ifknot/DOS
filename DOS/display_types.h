//#pragma once
#ifndef DISPLAY_TYPES_H
#define DISPLAY_TYPES_H

#include <stdint.h>
#include <string>
#include <vector>

namespace dos {

	/**
	 * Video BIOS INT 10 video modes.
	 */
	enum video_mode_t {
		TEXT_MONOCHROME_40X25 = 0,			// 00  40x25 B/W text (CGA,EGA,MCGA,VGA)
		TEXT_16_COLOUR_40X25,				// 01  40x25 16 color text (CGA,EGA,MCGA,VGA)
		TEXT_16_GRAY_80X25,					// 02  80x25 16 shades of gray text (CGA,EGA,MCGA,VGA)
		TEXT_16_COLOUR_80x25,				// 03  80x25 16 color text (CGA,EGA,MCGA,VGA)
		GRAPHICS_4_COLOUR_300x200,			// 04  320x200 4 color graphics (CGA,EGA,MCGA,VGA)
		GRAPHICS_4_GRAY_300x200,			// 05  320x200 4 color graphics (CGA,EGA,MCGA,VGA)
		GRAPHICS_MONOCHROME_640X200,		// 06  640x200 B/W graphics (CGA,EGA,MCGA,VGA)
		TEXT_MONOCHROME_80X25,				// 07  80x25 Monochrome text (MDA,HERC,EGA,VGA)
		PCJR_GRAPHICS_16_COLOUR_160X200,	// 08  160x200 16 color graphics (PCjr)
		PCJR_GRAPHICS_16_COLOUR_320X200,	// 09  320x200 16 color graphics (PCjr)
		PCJR_GRAPHICS_4_COLOUR_640X200,		// 0A  640x200 4 color graphics (PCjr)
		RESERVED1_EGA,						// 0B  Reserved (EGA BIOS function 11)
		RESERVED2_EGA,						// 0C  Reserved (EGA BIOS function 11)
		GRAPHICS_16_COLOUR_320X200,			// 0D  320x200 16 color graphics (EGA,VGA)
		GRAPHICS_16_COLOUR_640X200,			// 0E  640x200 16 color graphics (EGA,VGA)
		GRAPHICS_MONOCHROME_640X350,		// 0F  640x350 Monochrome graphics (EGA,VGA)
		/*
		10  640x350 16 color graphics (EGA or VGA with 128K)
			 640x350 4 color graphics (64K EGA)
		11  640x480 B/W graphics (MCGA,VGA)
		12  640x480 16 color graphics (VGA)
		13  320x200 256 color graphics (MCGA,VGA)
		8x  EGA, MCGA or VGA ignore bit 7, see below
		9x  EGA, MCGA or VGA ignore bit 7, see below

		if AL bit 7=1, prevents EGA,MCGA & VGA from clearing display
		N.B. function updates byte at 40:49;  bit 7 of byte 40:87
		*/
	};

	/**
	 * Video BIOS INT 10 services.
	 */
	enum video_service_t {
	  SET_VIDEO_MODE = 0,
	  SET_CURSOR_TYPE,
	  SET_CURSOR_POS,
	  READ_CURSOR_POS,
	  READ_LIGHT_PEN,
	  SEL_DISPLAY_PAGE,
	  SCROLL_PAGE_UP,
	  SCROLL_PAGE_DOWN,
	  READ_CHAR_ATTRIB,
	  WRITE_CHAR_ATTRIB,
	  WRITE_CHAR,
	  SET_COLOUR_PALETTE,
	  SET_PIXEL,
	  GET_PIXEL,
	  WRITE_TEXT_TELETYPE,
	  GET_VIDEO_STATE,
	  PALETTE_REGISTERS,	// Set/get palette registers (EGA/VGA)
	  CHAR_GENERATOR,		// Character generator routine (EGA/VGA)
	  VIDEO_SUB_CONFIG,		// Video subsystem configuration (EGA/VGA)
	  WRITE_STRING,			// Write string (BIOS after 1/10/86)
	  /*	
		INT 10,14 - Load LCD char font (convertible)
		INT 10,15 - Return physical display parms (convertible)
		INT 10,1A - Video Display Combination (VGA)
		INT 10,1B - Video BIOS Functionality/State Information (MCGA/VGA)
		INT 10,1C - Save/Restore Video State  (VGA only)
		INT 10,FE - Get DESQView/TopView Virtual Screen Regen Buffer
		INT 10,FF - Update DESQView/TopView Virtual Screen Regen Buffer
	  */
	};

	/**
	 * INT 10,12 - Video Subsystem Configuration (EGA/VGA).
	 */
	enum video_subsystem_t {
		CONFIG_INFO = 0x10
	/*
	BL = 10  return video configuration information
	on return:
	BH = 0 if color mode in effect
	   = 1 if mono mode in effect
	BL = 0 if 64k EGA memory
	   = 1 if 128k EGA memory
	   = 2 if 192k EGA memory
	   = 3 if 256k EGA memory
	CH = feature bits
	CL = switch settings

	BL = 20  select alternate print screen routine

	BL = 30  select scan lines for alphanumeric modes
	   AL = 0  200 scan lines
	      = 1  350 scan lines
	      = 2  400 scan lines
	on return:
	AL = 12

	BL = 31  select default palette loading
	AL = 0 enable default palette loading
	   = 1 disable default palette loading
	on return:
	AL = 12

	BL = 32  CPU access to video RAM
	AL = 0  enable CPU access to video RAM and I/O ports
	   = 1  disable CPU access to video RAM and I/O ports
	on return:
	AL = 12

	BL = 33  Gray scale summing
	AL = 0  enable gray scale summing
	   = 2  disable gray scale summing
	on return:
	AL = 12

	BL = 34  cursor emulation
	AL = 0  enable cursor emulation
	   = 1  disable cursor emulation
	on return:
	AL = 12

	BL = 35  PS/2 video display switching
	AL = 0 initial adapter video off
	   = 1 initial planar video on
	   = 2 switch active video off
	   = 3 switch inactive video on
	   ES:DX pointer to 128 byte save area (when AL = 0, 2 or 3)
	on return:
	AL = 12

	BL = 36  video refresh control
	AL = 0 enable refresh
	   = 1 disable refresh
	on return:
	AL = 12

	*/
	};

	/**
	 * Video BIOS get video mode return structure.
	 */
	struct video_state_t {
		uint8_t			columns;
		video_mode_t	mode;
		uint8_t			page;
	};

	/**
	 * Types of video adapter.
	 */
	enum video_adapter_t {
		NONE = 0,
		MDA,
		CGA,
		EGA,
		MCGA,
		VGA,
		HGC,
		HGC_PLUS,
		HCG_INCOLOR,
		OTHER
	};

	static char adapter_strings[10][255] = {
		"None",
		"MDA",
		"CGA",
		"EGA",
		"MCGA",
		"VGA",
		"Hercules",
		"Hercules Plus",
		"Hercules InColor",
		"Other"
	};

	/**
	 * Video adapter information.
	 */
	struct video_adapter_info_t {
		video_adapter_t type;
	};

}

#endif	// DISPLAY_TYPES_H
