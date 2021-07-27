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

#define MODE6_XY \
__asm	mov		ax, EVEN_LINES \
__asm	mov		bx, y \
__asm	mov		ax, EVEN_LINES \
__asm	mov		bx, y \
__asm	test	bx, 1h \
__asm	jz		YES \
__asm	mov		ax, ODD_LINES \
__asm	YES:	mov	es, ax \
__asm	mov		ax, x \
__asm	mov		cx, ax \
__asm	and		cx, 7h \
__asm	mov		dl, 80h \
__asm	shr		dl, cl \
__asm	shr		ax, 1 \
__asm	shr		ax, 1 \
__asm	shr		ax, 1 \
__asm and bx, 0FFFEh \
__asm	shl		bx, 1 \
__asm	shl		bx, 1 \
__asm	shl		bx, 1 \
__asm	mov		cx, bx \
__asm	shl		cx, 1 \
__asm	shl		cx, 1 \
__asm	add		bx, cx \
__asm	add		bx, ax

#define PLOT_OR		__asm	or		es:[bx], dl		; es:bx now pointing to correct byte and pixel in dl 

#define PLOT_XOR	__asm	xor		es:[bx], dl		; es:bx now pointing to correct byte and pixel in dl

#define CLIP_BORDER \
	if (x < 0) return; \
	if (y < 0) return; \
	if (x > 639) return; \
	if (y > 199) return;

#define CLIP_TORUS(yscale) \
	if (x < 0) x = 640 + x; \
	if (y < 0) y = yscale + y; \
	if (x > 639) x = x - 640; \
	if (y >= yscale) y = y - yscale;

namespace mode6 {

	enum plot_mode {plot_or, plot_xor };
	enum clip_mode { clip_border, clip_torus};
	enum scale_mode { scale_none, scale_third, scale_npx };

	template< int default_mode, int default_clip, int default_scale >
	class gfx {
	public:
		static inline void plot_point(int16_t x, int16_t y) {}
	};

	class gfx< plot_or, clip_border, scale_none > {
	public:
		static inline void plot_point(int16_t x, int16_t y) {
			CLIP_BORDER
			__asm {
				PLOT_PUSH
				MODE6_XY
				PLOT_OR
				PLOT_POP
			}
		}
	};

	class gfx< plot_xor, clip_border, scale_none > {
	public:
		static inline void plot_point(int16_t x, int16_t y) {
			CLIP_BORDER
				__asm {
				PLOT_PUSH
				MODE6_XY
				PLOT_XOR
				PLOT_POP
			}
		}
	};

	class gfx< plot_or, clip_torus, scale_none > {
	public:
		static inline void plot_point(int16_t x, int16_t y) {
			CLIP_TORUS(200)
				__asm {
				PLOT_PUSH
				MODE6_XY
				PLOT_OR
				PLOT_POP
			}
		}
	};

	class gfx< plot_xor, clip_torus, scale_none > {
	public:
		static inline void plot_point(int16_t x, int16_t y) {
			CLIP_TORUS(200)
				__asm {
				PLOT_PUSH
				MODE6_XY
				PLOT_XOR
				PLOT_POP
			}
		}
	};

	/*------------------------------------------------------*/

	class gfx< plot_or, clip_border, scale_third > {
	public:
		static inline void plot_point(int16_t x, int16_t y) {
			CLIP_BORDER
				__asm {
				PLOT_PUSH
				//SCALE_THIRD
				MODE6_XY
				PLOT_OR
				PLOT_POP
			}
		}
	};

	class gfx< plot_xor, clip_border, scale_third > {
	public:
		static inline void plot_point(int16_t x, int16_t y) {
			CLIP_BORDER
				__asm {
				PLOT_PUSH
				//SCALE_THIRD
				MODE6_XY
				PLOT_XOR
				PLOT_POP
			}
		}
	};

	class gfx< plot_or, clip_torus, scale_third > {
	public:
		static inline void plot_point(int16_t x, int16_t y) {
			CLIP_TORUS(200)
				__asm {
				PLOT_PUSH
				//SCALE_THIRD
				MODE6_XY
				PLOT_OR
				PLOT_POP
			}
		}
	};

	class gfx< plot_xor, clip_torus, scale_third > {
	public:
		static inline void plot_point(int16_t x, int16_t y) {
			CLIP_TORUS(200)
				__asm {
				PLOT_PUSH
				//SCALE_THIRD
				MODE6_XY
				PLOT_XOR
				PLOT_POP
			}
		}
	};


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

	template<int plot, int clip, int scale>
	void bresenham_circle(uint16_t xc, uint16_t yc, uint16_t r) {
		int16_t x = 0, y = r;
		int16_t d = 3 - 2 * r;
		gfx<plot, clip, scale>::plot_point(xc + x, yc + y);
		gfx<plot, clip, scale>::plot_point(xc - x, yc + y);
		gfx<plot, clip, scale>::plot_point(xc + x, yc - y);
		gfx<plot, clip, scale>::plot_point(xc - x, yc - y);
		gfx<plot, clip, scale>::plot_point(xc + y, yc + x);
		gfx<plot, clip, scale>::plot_point(xc - y, yc + x);
		gfx<plot, clip, scale>::plot_point(xc + y, yc - x);
		gfx<plot, clip, scale>::plot_point(xc - y, yc - x);
		while (y >= x) {		// for each pixel draw all eight pixels
			x++;
			if (d > 0) {		// check for decision parameter and correspondingly update d, x, y
				y--;
				d = d + 4 * (x - y) + 10;
			}
			else {
				d = d + 4 * x + 6;
			}
			gfx<plot, clip, scale>::plot_point(xc + x, yc + y);
			gfx<plot, clip, scale>::plot_point(xc - x, yc + y);
			gfx<plot, clip, scale>::plot_point(xc + x, yc - y);
			gfx<plot, clip, scale>::plot_point(xc - x, yc - y);
			gfx<plot, clip, scale>::plot_point(xc + y, yc + x);
			gfx<plot, clip, scale>::plot_point(xc - y, yc + x);
			gfx<plot, clip, scale>::plot_point(xc + y, yc - x);
			gfx<plot, clip, scale>::plot_point(xc - y, yc - x);
		}
	}

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
