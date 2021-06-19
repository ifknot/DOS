#pragma once

#include <stdint.h>

namespace mode6 {

	/**
	 * plot a pixel in mode 6 monochrome 640x200 B/W graphics (CGA,EGA,MCGA,VGA)
	 */
	void plot(uint16_t x, uint16_t y);

}
