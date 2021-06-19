#pragma once

#include <stdint.h>

namespace mode6 {

	/**
	 * plot a pixel in mode 6 monochrome 640x200 B/W graphics (CGA,EGA,MCGA,VGA)
	 */
	void plot(uint16_t x, uint16_t y);

	/**
	 * xor plot a pixel in mode 6 monochrome 640x200 B/W graphics (CGA,EGA,MCGA,VGA)
	 */
	void xor_plot(uint16_t x, uint16_t y);

	/**
	 * fast vertical line
	 */
	void vline(uint16_t x, uint16_t y1, uint16_t y2);

	/**
	 * fast horizontal line
	 */
	void hline(uint16_t x1, uint16_t x2, uint16_t y);

}
