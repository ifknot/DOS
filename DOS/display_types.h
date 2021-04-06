#pragma once

#include <stdint.h>

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
	  GET_VIDEO_MODE
	};

	/**
	 * Video BIOS get video mode return structure.
	 */
	struct video_state_t {
		uint8_t		columns;
		uint8_t		mode;
		uint8_t		page;
	};

}