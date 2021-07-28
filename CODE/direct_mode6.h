#pragma once

#include <stdint.h>
#include <cassert>

#define SCALE_NPX(y_, scale_) \
__asm	.8087			\
__asm	finit			\
__asm	fild	y_		\
__asm	fmul	scale_	\
__asm	fistp	y_		\
__asm	fwait

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

#define PLOT_OR		__asm	or		es:[bx], dl		; es:bx now pointing to correct byte and pixel in dl 

#define PLOT_XOR	__asm	xor		es:[bx], dl		; es:bx now pointing to correct byte and pixel in dl

#define EVEN_LINES	0B800h	
#define ODD_LINES	0BA00h

static float MODE6_SCALE = 0.3125;

/**
 * calculate row byte y/2 * 80 bytes per row
 *		= y * 40
 *		= y * 0x28
 *		= y * 101000
 *  i.e. 3 shl, add, 2 shl, add
 * 
		mov		ax, EVEN_LINES	; assume even lines segment
		mov		bx, y			; load y
		test	bx, 1h			; is it even ?
		jz		YES				; yes jump
		mov		ax, ODD_LINES	; otherwise load odd lines segment
YES:	mov		es, ax			; transer segment into es
		mov		ax, x			; load x
		mov		cx, ax			; copy of x
		and		cx, 7h			; mask off 0111 lower bits i.e.mod 8 (thanks powers of 2)
		mov		dl, 80h			; load dl with a single pixel at msb 10000000
		shr		dl, cl			; shift single bit along by x mod 8
		shr		ax, 1			; calculate column byte x / 8
		shr		ax, 1
		shr		ax, 1
		and		bx, 0FFFEh		; mask out even / odd row bit from y
		shl		bx, 1			; 8086 shift left 3 time
		shl		bx, 1
		shl		bx, 1
		mov		cx, bx			; temp result in cx
		shl		cx, 1			; 8086 shift left twice
		shl		cx, 1
		add		bx, cx			; add back into bx
		add		bx, ax			; add in column byte
	*/
#define MODE6_XY \
__asm	mov		ax, EVEN_LINES	\
__asm	mov		bx, y	\
__asm	mov		ax, EVEN_LINES	\
__asm	mov		bx, y	\
__asm	test	bx, 1h	\
__asm	jz		YES		\
__asm	mov		ax, ODD_LINES \
__asm	YES:	mov	es, ax	\
__asm	mov		ax, x	\
__asm	mov		cx, ax	\
__asm	and		cx, 7h	\
__asm	mov		dl, 80h	\
__asm	shr		dl, cl	\
__asm	shr		ax, 1	\
__asm	shr		ax, 1	\
__asm	shr		ax, 1	\
__asm	and bx, 0FFFEh	\
__asm	shl		bx, 1	\
__asm	shl		bx, 1	\
__asm	shl		bx, 1	\
__asm	mov		cx, bx	\
__asm	shl		cx, 1	\
__asm	shl		cx, 1	\
__asm	add		bx, cx	\
__asm	add		bx, ax

/**
 * Scale y to mode6 divide by multiplying by the reciprocal 200/640 
 * = 0.3125 best aprrox 0.333r = .01010101r (binary) - sl. vertically squished
 * with shifts and adds, round up to avoid the losses, 3 terms for 16 bit accuracy vs speed
 * from truncation when bits are shifted off to the left :
 *		y += (y + 2) >> 2
 *		y += (y + 8) >> 4
 *		y += (y + 128) >> 8
 */
#define MODE6_SCALE_THIRD(y_) \
__asm	mov		bx, y_	\
__asm	mov		cx, bx	\
__asm	add		cx, 2	\
__asm	shr		cx, 1	\
__asm	shr		cx, 1	\
__asm	add		bx, cx	\
__asm	mov		cx, bx	\
__asm	add		cx, 8	\
__asm	shr		cx, 1	\
__asm	shr		cx, 1	\
__asm	shr		cx, 1	\
__asm	shr		cx, 1	\
__asm	add		bx, cx	\
__asm	mov		cx, bx	\
__asm	add		cx, 128	\
__asm	shr		cx, 1	\
__asm	shr		cx, 1	\
__asm	shr		cx, 1	\
__asm	shr		cx, 1	\
__asm	shr		cx, 1	\
__asm	shr		cx, 1	\
__asm	shr		cx, 1	\
__asm	shr		cx, 1	\
__asm	add		bx, cx	\
__asm	shr		bx, 1	\
__asm	shr		bx, 1	\
__asm	mov		y_, bx	

#define CLIP_BORDER(xscale_, yscale_) \
	if (x < 0) return; \
	if (y < 0) return; \
	if (x >= xscale_) return; \
	if (y >= yscale_) return;

#define CLIP_TORUS(xscale_, yscale_) \
	if (x < 0) x = xscale_ + x; \
	if (y < 0) y = yscale_ + y; \
	if (x >= xscale_) x = x - xscale_; \
	if (y >= yscale_) y = y - yscale_;

namespace mode6 {

	enum plot_mode {plot_or, plot_xor };
	enum clip_mode { clip_border, clip_torus};
	enum scale_mode { scale_none, scale_third, scale_npx };

	// template factory compile time select optimised asm gfx function(s)
	/*------------------------------------------------------*/

	template< int default_mode, int default_clip, int default_scale >
	class gfx {
	public:
		static inline void plot_point(int16_t x, int16_t y) {}
	};

	class gfx< plot_or, clip_border, scale_none > {
	public:
		static inline void plot_point(int16_t x, int16_t y) {
			CLIP_BORDER(640, 200)
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
			CLIP_BORDER(640, 200)
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
			CLIP_TORUS(640, 200)
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
			CLIP_TORUS(640, 200)
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
			CLIP_BORDER(640, 600)
				__asm {
				PLOT_PUSH
				MODE6_SCALE_THIRD(y)
				MODE6_XY
				PLOT_OR
				PLOT_POP
			}
		}
	};

	class gfx< plot_xor, clip_border, scale_third > {
	public:
		static inline void plot_point(int16_t x, int16_t y) {
			CLIP_BORDER(640, 600)
				__asm {
				PLOT_PUSH
				MODE6_SCALE_THIRD(y)
				MODE6_XY
				PLOT_XOR
				PLOT_POP
			}
		}
	};

	class gfx< plot_or, clip_torus, scale_third > {
	public:
		static inline void plot_point(int16_t x, int16_t y) {
			CLIP_TORUS(640, 600)
				__asm {
				PLOT_PUSH
				MODE6_SCALE_THIRD(y)
				MODE6_XY
				PLOT_OR
				PLOT_POP
			}
		}
	};

	class gfx< plot_xor, clip_torus, scale_third > {
	public:
		static inline void plot_point(int16_t x, int16_t y) {
			CLIP_TORUS(640, 600)
				__asm {
				PLOT_PUSH
				MODE6_SCALE_THIRD(y)
				MODE6_XY
				PLOT_XOR
				PLOT_POP
			}
		}
	};

	/*------------------------------------------------------*/

	class gfx< plot_or, clip_border, scale_npx > {
	public:
		static inline void plot_point(int16_t x, int16_t y) {
			CLIP_BORDER(640, 640)
				__asm {
				PLOT_PUSH
				SCALE_NPX(y, MODE6_SCALE)
				MODE6_XY
				PLOT_OR
				PLOT_POP
			}
		}
	};

	class gfx< plot_xor, clip_border, scale_npx > {
	public:
		static inline void plot_point(int16_t x, int16_t y) {
			CLIP_BORDER(640, 640)
				__asm {
				PLOT_PUSH
				SCALE_NPX(y, MODE6_SCALE)
				MODE6_XY
				PLOT_XOR
				PLOT_POP
			}
		}
	};

	class gfx< plot_or, clip_torus, scale_npx > {
	public:
		static inline void plot_point(int16_t x, int16_t y) {
			CLIP_TORUS(640, 640)
				__asm {
				PLOT_PUSH
				SCALE_NPX(y, MODE6_SCALE)
				MODE6_XY
				PLOT_OR
				PLOT_POP
			}
		}
	};

	class gfx< plot_xor, clip_torus, scale_npx > {
	public:
		static inline void plot_point(int16_t x, int16_t y) {
			CLIP_TORUS(640, 640)
				__asm {
				PLOT_PUSH
				SCALE_NPX(y, MODE6_SCALE)
				MODE6_XY
				PLOT_XOR
				PLOT_POP
			}
		}
	};

	void fast_vertical_line(uint16_t x, uint16_t y1, uint16_t y2);

	void fast_horizontal_line(uint16_t x1, uint16_t x2, uint16_t y);

	void bresenham_line(int16_t x1, int16_t y1, int16_t x2, int16_t y2);

	void fast_box(uint16_t x, uint16_t y, uint16_t w, uint16_t h);

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

	void bresenham_oval();

}