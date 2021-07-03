#pragma once

namespace cga {

	/**
	 * 'warm' = Black, Red, Green and Yellow
	 * 'cool' = Black, Cyan, Magenta and White
	 */
	enum palette_t { warm = 0, cool };

	enum intensity_t { low = 0, high = 0x10 };

	enum colour_t {
		black = 0x00,
		cyan = 0x40,
		red = 0x40,
		magenta = 0x80,
		green = 0x80,
		white = 0xC0,
		yellow = 0xC0
	};

	typedef std::vector<colour_t> colour_vector_t;

}
