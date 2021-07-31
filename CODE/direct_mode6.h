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

#define LINE_PUSH \
__asm	push	ax		\
__asm	push	bx		\
__asm	push	cx		\
__asm	push	dx		\
__asm	push    di		\
__asm	push	si

#define LINE_POP \
__asm	pop		si		\
__asm	pop		di		\
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
#define MODE6_XY(x_, y_) \
__asm	mov		ax, EVEN_LINES	\
__asm	mov		bx, y_	\
__asm	mov		ax, EVEN_LINES	\
__asm	mov		bx, y_	\
__asm	test	bx, 1h	\
__asm	jz		YES		\
__asm	mov		ax, ODD_LINES \
__asm	YES:	mov	es, ax	\
__asm	mov		ax, x_	\
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

#define MODE6_BXY() \
__asm	mov		cx, EVEN_LINES	\
__asm	test	bx, 01h	\
__asm	jz		EVEN	\
__asm	mov		cx, ODD_LINES	\
__asm	EVEN:	mov	es, cx		\
__asm	mov		cx, ax	\
__asm	and		cx, 07h	\
__asm	mov		dl, 80h	\
__asm	shr		dl, cl	\	
__asm	shr		ax, 1	\
__asm	shr		ax, 1	\
__asm	shr		ax, 1	\
__asm	and		bx, 0FFFEh		\
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

#define CLIP_BORDER(x_, y_, xscale_, yscale_) \
	if (x_ < 0) return; \
	if (y_ < 0) return; \
	if (x_ >= xscale_) return; \
	if (y_ >= yscale_) return;

#define CLIP_TORUS(x_, y_, xscale_, yscale_) \
	if (x_ < 0) x_ = xscale_ + x_; \
	if (y_ < 0) y_ = yscale_ + y_; \
	if (x_ >= xscale_) x_ = x_ - xscale_; \
	if (y_ >= yscale_) y_ = y_ - yscale_;

namespace mode6 {

	inline void cls() {
		__asm {
			.8086

			mov		dx, 3DAh
L1:			in		al, dx
			test	al, 1
			jz		L1
			mov		ax, EVEN_LINES
			mov		es, ax
			xor		ax, ax
			xor		di, di
			cld
			mov		cx, 1000h
			rep		stosw
			mov		dx, 3DAh
L2:			in		al, dx
			test	al, 1
			jz		L2
			xor		ax, ax
			mov		cx, 1000h
			rep		stosw
		}
	}

	inline void sync() {
		__asm {
			.8086
			mov		dx, 3DAh
L1:			in		al, dx
			test	al, 1h
			jz		L1
		}
	}

	enum plot_mode {plot_or, plot_xor };
	enum clip_mode { clip_border, clip_torus};
	enum scale_mode { scale_none, scale_third, scale_npx };

	// template factory compile time select optimised asm gfx function(s)
	/*------------------------------------------------------*/

	template< int default_mode, int default_clip, int default_scale >
	class gfx {
	public:
		static inline void plot_point(int16_t x, int16_t y) {}

		static inline void draw_line(int16_t x1, int16_t y1, int16_t x2, int16_t y2) {}

	};

	class gfx< plot_or, clip_border, scale_none > {
	public:
		static inline void plot_point(int16_t x, int16_t y) {
			CLIP_BORDER(x, y, 640, 200)
			__asm {
				PLOT_PUSH
				MODE6_XY(x, y)
				PLOT_OR
				PLOT_POP
			}
		}

		//static inline void draw_line(int16_t x1, int16_t y1, int16_t x2, int16_t y2) {}

	};

	class gfx< plot_xor, clip_border, scale_none > {
	public:

		static inline void plot_point(int16_t x, int16_t y) {
			CLIP_BORDER(x, y, 640, 200)
				__asm {
				PLOT_PUSH
				MODE6_XY(x, y)
				PLOT_XOR
				PLOT_POP
			}
		}

		//static inline void draw_line(int16_t x1, int16_t y1, int16_t x2, int16_t y2) {}

	};

	class gfx< plot_or, clip_torus, scale_none > {
	public:

		static inline void plot_point(int16_t x, int16_t y) {
			CLIP_TORUS(x, y, 640, 200)
				__asm {
				PLOT_PUSH
				MODE6_XY(x, y)
				PLOT_OR
				PLOT_POP
			}
		}

		static inline void draw_line(int16_t x1, int16_t y1, int16_t x2, int16_t y2) {
			int16_t delta_x, delta_y, step_x, step_y, i1, i2;
			__asm {
				.8086
				LINE_PUSH

				// calculate deltas, steps and distance limits
				mov		ax, x1; load starting point x
				mov		bx, y1; load starting point y
				// is it a positive or negative x direction?
				mov		step_x, -1; assume - ve x direction
				sub		ax, x2; calculate delta x
				jge		NEGX; yes - ve x direction so calculate y direction
				neg		ax; abs(delta x)
				neg		step_x; +ve x direction
				// is it a positive or negative y direction?
				NEGX : mov		delta_x, ax; store delta x
				mov		step_y, -1; assume - ve y direction
				sub		bx, y2; calculate delta y
				jge		NEGY; yes - ve y direction
				neg		bx; abs(delta y)
				neg		step_y; +ve y direction
				NEGY : mov		delta_y, bx; store delta y
				// delta x in bx, delta y in ax
				inc		ax
				mov		cx, ax; save x repeat count
				shr		ax, 1; i1 = -delta_y + (delta_x + 1) div 2
				sub		ax, bx
				mov		i1, ax; store i1
				inc		bx
				mov		dx, bx; save y repeat count
				shr		bx, 1; i2 = delta_x - (delta_y + 1) div 2
				sub		bx, cx; adjust possible repeat count
				inc		bx
				neg		bx
				mov		i2, bx; store i1
				cmp		cx, dx; repeat count max(delta_x + 1, delta_y + 1)
				jge		BIGX; delta_x is bigger
				mov		cx, dx; delta_y is bigger
				// assume distance limit decision variable D = 0 and x,y start point
				// maximise use 8086 limited no. registers (6 ie AX BX CX DX SI DI) to improve performance 
				BIGX : mov		ax, x1; load x1 into ax
				mov		bx, y1; load y1 into bx
				mov		si, i1; load i1 into si
				xor		di, di; load D into di
				jmp		BPLOT; plot the first point
				// next pixel selected with the least distance from true line
				MORE : mov		dx, di; load decision variable D into dx
				//cmp		dx, i1		; 9 + 6EA cycles every loop
				cmp		dx, si; 3 reg, reg cycles every loop(20 % of reg, mem)
				jl		HZ; D too - ve so must be horizontal
				add		bx, step_y; vertical step
				sub		dx, delta_x; update decision variable D - delta_x
				cmp		di, i2; check old D diagonal move
				jg		NDIAG; no horizontal only
				HZ : add		ax, step_x; x = x + step_x
				add		dx, delta_y; update decision variable D + delta_y
				NDIAG : mov		di, dx; store D
				// inline plot pixel (x,y) as (ax,bx)
BPLOT:			PLOT_PUSH
				MODE6_XY(bx, ax)
				PLOT_OR
				PLOT_POP

				loop	MORE

				LINE_POP
			}
		}

	};

	class gfx< plot_xor, clip_torus, scale_none > {
	public:

		static inline void plot_point(int16_t x, int16_t y) {
			CLIP_TORUS(x, y, 640, 200)
				__asm {
				PLOT_PUSH
				MODE6_XY(x, y)
				PLOT_XOR
				PLOT_POP
			}
		}

		//static inline void draw_line(int16_t x1, int16_t y1, int16_t x2, int16_t y2) {}

	};

	/*------------------------------------------------------*/

	class gfx< plot_or, clip_border, scale_third > {
	public:

		static inline void plot_point(int16_t x, int16_t y) {
			CLIP_BORDER(x, y, 640, 600)
				__asm {
				PLOT_PUSH
				MODE6_SCALE_THIRD(y)
				MODE6_XY(x, y)
				PLOT_OR
				PLOT_POP
			}
		}

		//static inline void draw_line(int16_t x1, int16_t y1, int16_t x2, int16_t y2) {}

	};

	class gfx< plot_xor, clip_border, scale_third > {
	public:

		static inline void plot_point(int16_t x, int16_t y) {
			CLIP_BORDER(x, y, 640, 600)
				__asm {
				PLOT_PUSH
				MODE6_SCALE_THIRD(y)
				MODE6_XY(x, y)
				PLOT_XOR
				PLOT_POP
			}
		}

		//static inline void draw_line(int16_t x1, int16_t y1, int16_t x2, int16_t y2) {}

	};

	class gfx< plot_or, clip_torus, scale_third > {
	public:

		static inline void plot_point(int16_t x, int16_t y) {
			CLIP_TORUS(x, y, 640, 600)
				__asm {
				PLOT_PUSH
				MODE6_SCALE_THIRD(y)
				MODE6_XY(x, y)
				PLOT_OR
				PLOT_POP
			}
		}

		//static inline void draw_line(int16_t x1, int16_t y1, int16_t x2, int16_t y2) {}

	};

	class gfx< plot_xor, clip_torus, scale_third > {
	public:

		static inline void plot_point(int16_t x, int16_t y) {
			CLIP_TORUS(x, y, 640, 600)
				__asm {
				PLOT_PUSH
				MODE6_SCALE_THIRD(y)
				MODE6_XY(x, y)
				PLOT_XOR
				PLOT_POP
			}
		}

		//static inline void draw_line(int16_t x1, int16_t y1, int16_t x2, int16_t y2) {}

	};

	/*------------------------------------------------------*/

	class gfx< plot_or, clip_border, scale_npx > {
	public:

		static inline void plot_point(int16_t x, int16_t y) {
			CLIP_BORDER(x, y, 640, 640)
				__asm {
				PLOT_PUSH
				SCALE_NPX(y, MODE6_SCALE)
				MODE6_XY(x, y)
				PLOT_OR
				PLOT_POP
			}
		}

		//static inline void draw_line(int16_t x1, int16_t y1, int16_t x2, int16_t y2) {}

	};

	class gfx< plot_xor, clip_border, scale_npx > {
	public:

		static inline void plot_point(int16_t x, int16_t y) {
			CLIP_BORDER(x, y, 640, 640)
				__asm {
				PLOT_PUSH
				SCALE_NPX(y, MODE6_SCALE)
				MODE6_XY(x, y)
				PLOT_XOR
				PLOT_POP
			}
		}

		//static inline void draw_line(int16_t x1, int16_t y1, int16_t x2, int16_t y2) {}

	};

	class gfx< plot_or, clip_torus, scale_npx > {
	public:

		static inline void plot_point(int16_t x, int16_t y) {
			CLIP_TORUS(x, y, 640, 640)
				__asm {
				PLOT_PUSH
				SCALE_NPX(y, MODE6_SCALE)
				MODE6_XY(x, y)
				PLOT_OR
				PLOT_POP
			}
		}

		//static inline void draw_line(int16_t x1, int16_t y1, int16_t x2, int16_t y2) {}

	};

	class gfx< plot_xor, clip_torus, scale_npx > {
	public:

		static inline void plot_point(int16_t x, int16_t y) {
			CLIP_TORUS(x, y, 640, 640)
				__asm {
				PLOT_PUSH
				SCALE_NPX(y, MODE6_SCALE)
				MODE6_XY(x, y)
				PLOT_XOR
				PLOT_POP
			}
		}

		//static inline void draw_line(int16_t x1, int16_t y1, int16_t x2, int16_t y2) {}

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

/*

// ax column byte, bx 80 byte row, dl pixel bit
				mov		cx, EVEN_LINES
				//mov		bx, y		; load y
				test	bx, 01h; is it an odd row ?
				jz		EVEN; no keep even lines offset
				mov		cx, ODD_LINES
				EVEN : mov		es, cx; offset into extended segment
				// mode 6 is 1 bit per pixel
				// bit to set within column byte is x mod 8
				//mov		ax, x		; load x
				mov		cx, ax; copy x
				and cx, 07h; mask off 0111 lower bits(mod 8)
				mov		dl, 80h; load dl with 10000000
				shr		dl, cl; shift single bit along by x mod 8
				// column byte is x/8
				shr		ax, 1; 8086 shift right 3 times
				shr		ax, 1
				shr		ax, 1
				// row	= y/2 * 80 bytes per row
				//		= y * 40
				//		= y * 0x28
				//		= y * 101000 = 3 shl, add, 2 shl, add
				and bx, 0FFFEh; remove even / odd row bit from y
				shl		bx, 1; 8086 shift left 3 time
				shl		bx, 1
				shl		bx, 1
				mov		cx, bx; put result in cx
				shl		cx, 1; 8086 shift left twice
				shl		cx, 1
				add		bx, cx; add back into bx
				add		bx, ax; add in column byte

*/
