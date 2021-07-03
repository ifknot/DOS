#pragma once

#include <stdint.h>

#include "cga_bitmap.h"

namespace mode4 {

	//void select_pallette(palette);

	//void set_brightness(intensity);

	void plot(uint16_t x, uint16_t y, cga::colour_t colour);

	void xor_plot(uint16_t x, uint16_t y, cga::colour_t colour);

	//void copy(cga::bitmap& bmp);

	//void cut(cga::bitmap& bmp);

	uint16_t paste(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t data[], uint16_t size);

	//void vline(uint16_t x, uint16_t y1, uint16_t y2);

	//void xor_vline(uint16_t x, uint16_t y1, uint16_t y2);

	//void hline(uint16_t x1, uint16_t x2, uint16_t y);

	//void xor_hline(uint16_t x1, uint16_t x2, uint16_t y);

	//void bline(int16_t x1, int16_t y1, int16_t x2, int16_t y2);

	//void box(uint16_t x, uint16_t y, uint16_t w, uint16_t h);

}
