#pragma once

#include <stdint.h>

#include <cassert>

#define EVEN_LINES	0B800h	
#define ODD_LINES	0BA00h

#define PLOT_PUSH \
__asm	.8086			\
__asm	push	ax		\
__asm	push	bx		\
__asm	push	cx		\
__asm	push	dx		\
__asm	push    es		

#define PLOT_POP \
__asm	pop		es		\
__asm	pop		dx		\
__asm	pop		cx		\
__asm	pop		bx		\
__asm	pop		ax		

#define OR_PIXEL	__asm	or		es:[bx], dl		; es:bx now pointing to correct byte and pixel in dl 

#define XOR_PIXEL	__asm	xor		es:[bx], dl		; es:bx now pointing to correct byte and pixel in dl

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

	void fast_box(uint16_t x, uint16_t y, uint16_t w, uint16_t h);

	void fast_box_xor(uint16_t x, uint16_t y, uint16_t w, uint16_t h);

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
