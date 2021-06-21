#pragma once

#include <stdint.h>

namespace random {

	inline uint32_t xorshift32(uint32_t seed = 2463534242UL) {
		static uint32_t y = seed;
		y ^= (y << 13); y ^= (y >> 17); 
		return (y ^= (y << 15));
	}

}
