#pragma once

#include <stdint.h>
#include <cassert>

#define EVEN_LINES	0B800h	
#define ODD_LINES	0BA00h

#define CONTROL_REG 3D8h
#define STATUS_REG	3DAh

static float MODE6_SCALE = 0.3125;

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

#define PLOT_OR		or		es:[bx], dl

#define PLOT_XOR	xor		es:[bx], dl

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

#define MODE6_BXY \
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

#define DELTAS \
__asm	mov		ax, x1	\
__asm	mov		bx, y1	\
__asm	mov		step_x, -1		\
__asm	sub		ax, x2	\
__asm	jge		NEGX	\
__asm	neg		ax		\
__asm	neg		step_x	\
__asm	NEGX:	mov	delta_x, ax	\
__asm	mov		step_y, -1		\
__asm	sub		bx, y2	\
__asm	jge		NEGY	\
__asm	neg		bx		\
__asm	neg		step_y	\
__asm	NEGY:	mov	delta_y, bx	\
__asm	inc		ax		\
__asm	mov		cx, ax	\
__asm	shr		ax, 1	\
__asm	sub		ax, bx	\
__asm	mov		i1, ax	\
__asm	inc		bx		\
__asm	mov		dx, bx	\
__asm	shr		bx, 1	\
__asm	sub		bx, cx	\
__asm	inc		bx		\
__asm	neg		bx		\
__asm	mov		i2, bx	\
__asm	cmp		cx, dx	\
__asm	jge		BIGX	\
__asm	mov		cx, dx	\
__asm	BIGX:	mov	ax, x1		\
__asm	mov		bx, y1	\
__asm	mov		si, i1	\
__asm	xor		di, di		

#define LEASTD \
__asm	mov		dx, di	\
__asm	cmp		dx, si	\
__asm	jl		HZ		\
__asm	add		bx, step_y		\
__asm	sub		dx, delta_x		\
__asm	cmp		di, i2	\
__asm	jg		NDIAG	\
__asm	HZ:		add	ax, step_x	\
__asm	add		dx, delta_y		\
__asm	NDIAG:	mov	di, dx

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

#define SCREEN_OFF \
__asm	mov		dx, CONTROL_REG	\
__asm	in		al, dx			\
__asm	and		al, 11110111b	\
__asm	out		dx, al	

#define SCREEN_OFF \
__asm	push	ax	\
__asm	push	dx	\
__asm	mov		dx, CONTROL_REG	\
__asm	in		al, dx			\
__asm	and		al, 11110111b	\
__asm	out		dx, al			\
__asm	pop		dx	\
__asm	pop		ax	

#define SCREEN_ON \
__asm	push	ax	\
__asm	push	dx	\
__asm	mov		dx, CONTROL_REG	\
__asm	in		al, dx			\
__asm	or		al, 00000100b	\
__asm	out		dx, al			\
__asm	pop		dx	\
__asm	pop		ax	

#define SYNC \
__asm	push	ax	\
__asm	push	dx	\
__asm	mov		dx, STATUS_REG	\
__asm	SNOW:	in	al, dx	\		
__asm	test	al, 8		\
__asm	je		SNOW		\
__asm	pop		dx	\
__asm	pop		ax

namespace mode6 {

	inline void cls() {
		__asm {
			.8086

			push	ax
			push	cx
			push	di
			push	es

			mov		ax, EVEN_LINES
			mov		es, ax				; video RAM segment address
			xor		di, di				; zero destination index
			cld							; set increment mode
			mov		cx, 2000h			; 16K video ram in word steps
			
			xor		ax, ax				; zero ax
			rep		stosw				; fill video ram with 0
			
			pop		es
			pop		di
			pop		cx
			pop		ax
		}
	}

	inline void sync() {
		__asm {
			.8086
			push	ax	
			push	dx

			mov		dx, STATUS_REG	
	SNOW:	in		al, dx	
			test	al, 8		
			je		SNOW

			pop		dx	
			pop		ax
		}
	}

	inline void spin_wait(uint16_t delay) {
		__asm {
			.8086
			push	cx
			
			mov		cx, delay
L1:			nop
			loop	L1

			pop		cx
		}
	}

	inline void show_wait(uint16_t delay) {
		__asm {
			.8086
			push	ax	
			push	cx
			push	dx			

			mov		dx, CONTROL_REG
			in		al, dx
			or		al, 00000100b		; screen on
			out		dx, al

			mov		cx, delay			
L1:			nop
			loop	L1

			mov		dx, CONTROL_REG
			in		al, dx
			and al, 11110111b			; screen off
			out		dx, al

			pop		dx
			pop		cx
			pop		ax
		}
	}

	inline void screen_off() {
		__asm {
			.8086
			push	ax
			push	dx

			mov		dx, CONTROL_REG
			in		al, dx
			and al, 11110111b			; screen off
			out		dx, al

			pop		dx
			pop		ax
		}
	}

	inline void screen_on() {
		__asm {
			.8086
			push	ax
			push	dx

			mov		dx, CONTROL_REG
			in		al, dx
			or		al, 00000100b		; screen on
			out		dx, al

			pop		dx
			pop		ax
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
				DELTAS
				//SCREEN_OFF
				jmp		BPLOT; plot the first point
MORE:			LEASTD			
BPLOT:			PLOT_PUSH
				MODE6_BXY
				PLOT_OR
				PLOT_POP
				loop	MORE
				//SCREEN_ON
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

		static inline void draw_line(int16_t x1, int16_t y1, int16_t x2, int16_t y2) {
			int16_t delta_x, delta_y, step_x, step_y, i1, i2;
			__asm {
				.8086
				LINE_PUSH
				DELTAS
				//SCREEN_OFF
				jmp		BPLOT; plot the first point
MORE:			LEASTD
BPLOT:			PLOT_PUSH
				MODE6_BXY
				PLOT_XOR
				PLOT_POP
				loop	MORE
				//SCREEN_ON
				LINE_POP
			}
		}

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
