#pragma once

#include <stdint.h>

namespace mode4 {

	enum palette_t { warm = 0, cool };

	enum intensity_t { low = 0, high = 0x10 };

	enum colour_t { black, cyan = 0x40, magenta = 0x80, white = 0xC0
	};

#define red		colour_t::cyan
#define green	colour_t::magenta
#define yellow	colour_t::white

	/**
	 * 'warm' = Black, Red, Green and Yellow
	 * 'cool' = Black, Cyan, Magenta and White
	 */
	//void select_pallette(palette);

	//void set_brightness(intensity);

	void plot(uint16_t x, uint16_t y, colour_t colour);

	//void xor_plot(uint16_t x, uint16_t y);

	//void vline(uint16_t x, uint16_t y1, uint16_t y2);

	//void xor_vline(uint16_t x, uint16_t y1, uint16_t y2);

	//void hline(uint16_t x1, uint16_t x2, uint16_t y);

	//void xor_hline(uint16_t x1, uint16_t x2, uint16_t y);

	//void bline(int16_t x1, int16_t y1, int16_t x2, int16_t y2);

	//void box(uint16_t x, uint16_t y, uint16_t w, uint16_t h);

}
