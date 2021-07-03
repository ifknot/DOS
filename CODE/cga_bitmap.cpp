#include "cga_bitmap.h"

#include <cassert>

namespace cga {

	bitmap::bitmap(dim_t d) {
		assert(d.width % 2 == 0);
		resize(d);
	}

	void bitmap::resize(dim_t d) {
		dim.width = d.width / 4; // pixels per byte
		dim.height = d.height / 2; // odd plane and even plane
		odd.resize(dim.width * dim.height);
		even.resize(odd.capacity());
	}

	void bitmap::fill(colour_t c) {
		byte_t b = c;
		b <<= 2;
		b |= c;
		b <<= 2;
		b |= c;
		b <<= 2;
		b |= c;
		for (int i = 0; i < odd.capacity(); ++i) {
			odd[i] = b;
			even[i] = b;
		}
	}

	void bitmap::clear() {
		for (int i = 0; i < odd.capacity(); ++i) {
			odd[i] = 0;
			even[i] = 0;
		}
	}

}
