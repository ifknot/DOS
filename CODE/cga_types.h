#pragma once

#include <stdint.h>
#include <vector>

namespace cga {

	typedef uint8_t byte_t;

	typedef union word_t {
		uint16_t word;
		struct {
			uint8_t lo;
			uint8_t hi;
		} byte;
	} word_t;

	typedef float fp_t;

	typedef std::vector<byte_t> byte_vector_t;

	struct point_t {

		point_t() : x(0), y(0) {}

		point_t(uint16_t x, uint16_t y) : x(x), y(y) {}

		uint16_t x;
		uint16_t y;

	};

	struct dim_t {

		dim_t() : width(0), height(0) {}

		dim_t(uint16_t width, uint16_t height) : width(width), height(height) {}

		uint16_t width;
		uint16_t height;

	};

	struct rect_t {

		point_t point;
		dim_t	dim;

	};

}
