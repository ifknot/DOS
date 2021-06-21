#pragma once

#include <stdint.h>

namespace mode6 {

	void plot(uint16_t x, uint16_t y);

	void xor_plot(uint16_t x, uint16_t y);

	void vline(uint16_t x, uint16_t y1, uint16_t y2);

	void xor_vline(uint16_t x, uint16_t y1, uint16_t y2);

	void hline(uint16_t x1, uint16_t x2, uint16_t y);

	void xor_hline(uint16_t x1, uint16_t x2, uint16_t y);

	void bline(int16_t x0, int16_t y0, int16_t x1, int16_t y1);

	//void box(uint16_t x, uint16_t y, uint16_t w, unint16_t h);

}
