#pragma once

#include <stdint.h>

namespace mode6 {

	void plot_point(uint16_t x, uint16_t y);

	void plot_point_xor(uint16_t x, uint16_t y);

	void fast_vertical_line(uint16_t x, uint16_t y1, uint16_t y2);

	void fast_vertical_line_xor(uint16_t x, uint16_t y1, uint16_t y2);

	void fast_horizontal_line(uint16_t x1, uint16_t x2, uint16_t y);

	void fast_horizontal_line_xor(uint16_t x1, uint16_t x2, uint16_t y);

	void bresenham_line(int16_t x1, int16_t y1, int16_t x2, int16_t y2);

	void bresenham_line_xor(int16_t x1, int16_t y1, int16_t x2, int16_t y2);

	void fast_box(uint16_t x, uint16_t y, uint16_t w, uint16_t h);

	void fast_box_xor(uint16_t x, uint16_t y, uint16_t w, uint16_t h);

	void bresenham_circle(uint16_t xc, uint16_t yc, uint16_t r);

	void bresenham_circle_xor(uint16_t xc, uint16_t yc, uint16_t r);

	void bresenham_oval();

	void bresenham_oval_xor();

}

namespace mode6_scaled {

	void plot_point(uint16_t x, uint16_t y);

	void plot_point_xor(uint16_t x, uint16_t y);

	void fast_vertical_line(uint16_t x, uint16_t y1, uint16_t y2);

	void fast_vertical_line_xor(uint16_t x, uint16_t y1, uint16_t y2);

	void fast_horizontal_line(uint16_t x1, uint16_t x2, uint16_t y);

	void fast_horizontal_line_xor(uint16_t x1, uint16_t x2, uint16_t y);

	void bresenham_line(int16_t x1, int16_t y1, int16_t x2, int16_t y2);

	void bresenham_line_xor(int16_t x1, int16_t y1, int16_t x2, int16_t y2);

	void box(uint16_t x, uint16_t y, uint16_t w, uint16_t h);

	void box_xor(uint16_t x, uint16_t y, uint16_t w, uint16_t h);

	void bresenham_circle(uint16_t xc, uint16_t yc, uint16_t r);

	void bresenham_circle_xor(uint16_t xc, uint16_t yc, uint16_t r);

	void bresenham_oval();

	void bresenham_oval_xor();

}

namespace mode6_scaled_npx {

	void plot_point(uint16_t x, uint16_t y);

	void plot_point_xor(uint16_t x, uint16_t y);

	void fast_vertical_line(uint16_t x, uint16_t y1, uint16_t y2);

	void fast_vertical_line_xor(uint16_t x, uint16_t y1, uint16_t y2);

	void fast_horizontal_line(uint16_t x1, uint16_t x2, uint16_t y);

	void fast_horizontal_line_xor(uint16_t x1, uint16_t x2, uint16_t y);

	void bresenham_line(int16_t x1, int16_t y1, int16_t x2, int16_t y2);

	void box(uint16_t x, uint16_t y, uint16_t w, uint16_t h);

	void box_xor(uint16_t x, uint16_t y, uint16_t w, uint16_t h);

	void bresenham_circle(uint16_t xc, uint16_t yc, uint16_t r);

	void bresenham_circle_xor(uint16_t xc, uint16_t yc, uint16_t r);

	void bresenham_oval();

	void bresenham_oval_xor();

}
