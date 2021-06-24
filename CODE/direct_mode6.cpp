#include "direct_mode6.h"

#include <cassert>

#define EVEN_LINES	0B800h	// even lines video buffer memory	
#define ODD_LINES	0BA00h	// odd lines video buffer memory

namespace mode6 {

	void plot(uint16_t x, uint16_t y) {
		assert(x < 640 && y < 200);
		__asm {
			.8086
			push	ax
			push	bx
			push	cx
			push	dx
			push    es

			// ax column byte, bx 80 byte row, dl pixel bit 
			mov		ax, EVEN_LINES	
			mov		bx, y		; load y
			test	bx, 01h		; is it an odd row?
			jz		EVEN		; no keep even lines offset
			mov		ax, ODD_LINES	
	EVEN:	mov		es, ax		; offset into extended segment
			// mode 6 is 1 bit per pixel
			// bit to set within column byte is x mod 8
			mov		ax, x		; load x
			mov		cx, ax		; copy x
			and		cx, 07h		; mask off 0111 lower bits (mod 8)
			mov		dl, 080h	; load dl with 10000000
			shr		dl, cl		; shift single bit along by x mod 8
			// column byte is x/8
			shr		ax, 1		; 8086 shift right 3 times
			shr		ax, 1
			shr		ax, 1
			// row	= y/2 * 80 bytes per row
			//		= y * 40
			//		= y * 0x28
			//		= y * 101000 = 3 shl, add, 2 shl, add
			and		bx, 0FFFEh	; remove even / odd row bit from y
			shl		bx, 1		; 8086 shift left 3 time
			shl		bx, 1
			shl		bx, 1
			mov		cx, bx		; put result in cx
			shl		cx, 1		; 8086 shift left twice
			shl		cx, 1
			add		bx, cx		; add back into bx
			add		bx, ax		; add in column byte
			or		es:[bx], dl	; set pixel bit in video buffer

			pop es
			pop	dx
			pop	cx
			pop	bx
			pop	ax
		}
	}

	void xor_plot(uint16_t x, uint16_t y) {
		assert(x < 640 && y < 200);
		__asm {
			.8086
			push	ax
			push	bx
			push	cx
			push	dx
			push    es

			mov		ax, EVEN_LINES	
			mov		bx, y 
			test	bx, 01h 
			jz		EVEN 
			mov		ax, ODD_LINES 
			EVEN :	mov		es, ax 
			mov		ax, x 
			mov		cx, ax 
			and cx, 07h 
			mov		dl, 080h 
			shr		dl, cl 
			shr		ax, 1 
			shr		ax, 1 
			shr		ax, 1 
			and bx, 0FFFEh 
			shl		bx, 1 
			shl		bx, 1 
			shl		bx, 1 
			mov		cx, bx 
			shl		cx, 1 
			shl		cx, 1 
			add		bx, cx 
			add		bx, ax 
			xor es: [bx] , dl

			pop es
			pop	dx
			pop	cx
			pop	bx
			pop	ax
		}
	}

	void vline(uint16_t x, uint16_t y1, uint16_t y2) {
		assert(x < 640 && y1 < 200 && y2 < 200 && y1 <= y2);
		__asm {
			.8086
			push	ax
			push	bx
			push	cx
			push	dx
			push    es

			// starting page odd/even
			mov		ax, EVEN_LINES	
			mov		bx, y1		; load y1
			test	bx, 01h		; is it an odd row?
			pushf				; preserve odd/even state
			jz		KEEP		; no keep even lines offset
			mov		ax, ODD_LINES	
	KEEP:	mov		es, ax		; offset into extended segment
			// dl pixel bit
			mov		ax, x		; load x
			mov		cx, ax		; copy x
			and		cx, 07h		; mask off 0111 lower bits (mod 8)
			mov		dl, 080h	; load dl with 10000000
			shr		dl, cl		; shift single bit along by x mod 8
			// ax column byte
			shr		ax, 1		; 8086 shift right 3 times
			shr		ax, 1
			shr		ax, 1
			// row	= y/2 * 80 bytes per row
			//		= y * 40
			//		= y * 0x28
			//		= y * 101000 = 3 shl, add, 2 shl, add
			and		bx, 0FFFEh	; remove even / odd row bit from y
			shl		bx, 1		; 8086 shift left 3 time
			shl		bx, 1
			shl		bx, 1
			mov		cx, bx		; put result in cx
			shl		cx, 1		; 8086 shift left twice
			shl		cx, 1
			add		bx, cx		; add back into bx
			add		bx, ax		; add in column byte
			or		es:[bx], dl	; set pixel bit in video buffer
			mov		cx, y2		
			sub		cx, y1		; cx is line length
			jz		END			; zero length (single pixel)
			popf				; retrieve odd/even state
			jz		EVEN		; 
			// plot vertical points starting in odd segment
			add		bx, 50h		; post-increment bx to next row

	ODD:	mov		ax, es
			xor		ax, 200h	; swap to opposite row video buffer
			mov		es, ax

			or		es:[bx], dl ; set pixel bit in video buffer
			dec		cx
			jz		END

			mov		ax, es
			xor		ax, 200h	; swap to opposite row video buffer
			mov		es, ax

			or		es:[bx], dl	; set pixel bit in video buffer
			add		bx, 50h		; post-increment bx to next row

			loop	ODD
			jmp		END
			// plot vertical points starting in even segment
	EVEN:	mov		ax, es
			xor		ax, 200h	; swap to opposite row video buffer
			mov		es, ax

			or		es:[bx], dl ; set pixel bit in video buffer
			dec		cx
			jz		END

			mov		ax, es
			xor		ax, 200h	; swap to opposite row video buffer
			mov		es, ax

			add		bx, 50h		; pre-increment bx to next row
			or		es:[bx], dl	; set pixel bit in video buffer

			loop	EVEN

	END:	pop es
			pop	dx
			pop	cx
			pop	bx
			pop	ax
		}
	}

	void xor_vline(uint16_t x, uint16_t y1, uint16_t y2) {
		assert(x < 640 && y1 < 200 && y2 < 200 && y1 <= y2);
		__asm {
			.8086
			push	ax
			push	bx
			push	cx
			push	dx
			push    es

			mov		ax, EVEN_LINES	
			mov		bx, y1		; load y1
			test	bx, 01h		; is it an odd row?
			pushf				; preserve odd/even state
			jz		KEEP		; no keep even lines offset
			mov		ax, ODD_LINES	
			KEEP:	mov		es, ax		; offset into extended segment	

			mov		ax, x		; load x
			mov		cx, ax		; copy x
			and		cx, 07h		; mask off 0111 lower bits (mod 8)
			mov		dl, 080h	; load dl with 10000000
			shr		dl, cl		; shift single bit along by x mod 8		
			
			shr		ax, 1		; 8086 shift right 3 times
			shr		ax, 1		; x / 8
			shr		ax, 1	

			and		bx, 0FFFEh	; remove even / odd row bit from y
			shl		bx, 1		; 8086 shift left 3 time
			shl		bx, 1
			shl		bx, 1
			mov		cx, bx		; put result in cx
			shl		cx, 1		; 8086 shift left twice
			shl		cx, 1
			add		bx, cx		; add back into bx
			add		bx, ax		; add in column byte
			xor		es:[bx], dl	; set pixel bit in video buffer

			mov		cx, y2		
			sub		cx, y1		; cx is line length
			jz		END			; zero length (single pixel)
			popf				; retrieve odd/even state
			jz		EVEN
			add		bx, 50h		; increment bx to next row

	ODD:	mov		ax, es
			xor		ax, 200h	; swap to opposite row video buffer
			mov		es, ax
			xor		es:[bx], dl ; set pixel bit in video buffer
			dec		cx
			jz		END
			mov		ax, es
			xor		ax, 200h	; swap to opposite row video buffer
			mov		es, ax
			xor		es:[bx], dl	; set pixel bit in video buffer
			add		bx, 50h; increment bx to next row
			loop	ODD
			jmp		END

	EVEN:	mov		ax, es
			xor		ax, 200h	; swap to opposite row video buffer
			mov		es, ax
			xor		es:[bx], dl ; set pixel bit in video buffer
			dec		cx
			jz		END
			add		bx, 50h		; increment bx to next row
			mov		ax, es
			xor		ax, 200h	; swap to opposite row video buffer
			mov		es, ax
			xor		es:[bx], dl	; set pixel bit in video buffer
			loop	EVEN

	END:	pop es
			pop	dx
			pop	cx
			pop	bx
			pop	ax
		}
	}

	void hline(uint16_t x1, uint16_t x2, uint16_t y) {
		assert(x1 < 640 && x2 < 640 && y < 200 && x1 <= x2);
		__asm {
			.8086
			push	ax
			push	bx
			push	cx
			push	dx
			push    es

			mov		ax, EVEN_LINES	
			mov		bx, y		; load y
			test	bx, 01h		; is it an odd row?
			jz		EVEN		; no keep even lines offset
			mov		ax,  ODD_LINES	
	EVEN:	mov		es, ax		; offset into extended segment
			// construct left most byte
			mov		ax, x1		; load x1
			mov		cx, ax		; copy x1
			and		cx, 07h		; mask off 0111 lower bits (mod 8)
			mov		dl, 0FFh	; load dl with 1111111
			shr		dl, cl		; shift single bit along by x mod 8
			// ax column byte
			shr		ax, 1		; 8086 shift right 3 times
			shr		ax, 1
			shr		ax, 1
			// row	= y/2 * 80 bytes per row
			//		= y * 40
			//		= y * 0x28
			//		= y * 101000 = 3 shl, add, 2 shl, add
			and		bx, 0FFFEh	; remove even / odd row bit from y
			shl		bx, 1		; 8086 shift left 3 time
			shl		bx, 1
			shl		bx, 1
			mov		cx, bx		; put result in cx
			shl		cx, 1		; 8086 shift left twice
			shl		cx, 1
			add		bx, cx		; add back into bx
			add		bx, ax		; add in column byte
			// line length fit in a single byte? (x1 % 8) + x2 - x1 < 8?
			mov		cx, x1		; load x1
			and		cx, 07h		; mod 8
			add		cx, x2
			sub		cx, x1

			cmp		cx, 08h
			jl		LEFT		; only plot left byte
			or		es:[bx], dl	; plot left most byte
			mov		dl, 0FFh	; load a full byte horizontal line
			// line length overlap 2 bytes? (x1 % 8) + x2 - x1 < 16?
			cmp		cx, 10h
			jl		RIGHT		; plot right most byte as well
			shr		cx, 1		; divide by 8 
			shr		cx, 1
			shr		cx, 1
			dec		cx
	FILL:	inc		bx			; point to next byte along
			or		es:[bx], dl ; plot filler byte
			loop	FILL
			// construct the right most byte
	RIGHT:	inc		bx
	LEFT:	mov		cx, 07h		; load cx with 0000000000000111
			sub		cx, x2		; subtract x2
			and		cl, 07h		; mask off 0111 lower bits(mod 8)
			mov		dh, 0FFh	; load dh with 1111111
			shl		dh, cl		; shift x2 mod 8
			and		dl, dh		; mask off dl
			// plot the right most byte
			or		es:[bx], dl	; set pixel bit in video buffer

	END:	pop es
			pop	dx
			pop	cx
			pop	bx
			pop	ax
		}
	}

	void xor_hline(uint16_t x1, uint16_t x2, uint16_t y) {
		assert(x1 < 640 && x2 < 640 && y < 200 && x1 <= x2);
		__asm {
			.8086
			push	ax
			push	bx
			push	cx
			push	dx
			push    es

			mov		ax, EVEN_LINES
			mov		bx, y		; load y
			test	bx, 01h		; is it an odd row ?
			jz		EVEN		; no keep even lines offset
			mov		ax,  ODD_LINES
	EVEN:	mov		es, ax		; offset into extended segment
			// construct left most byte
			mov		ax, x1		; load x1
			mov		cx, ax		; copy x1
			and		cx, 07h		; mask off 0111 lower bits(mod 8)
			mov		dl, 0FFh	; load dl with 1111111
			shr		dl, cl		; shift single bit along by x mod 8
			// ax column byte
			shr		ax, 1		; 8086 shift right 3 times
			shr		ax, 1
			shr		ax, 1
			// row	= y/2 * 80 bytes per row
			//		= y * 40
			//		= y * 0x28
			//		= y * 101000 = 3 shl, add, 2 shl, add
			and		bx, 0FFFEh	; mask out even / odd row bit from y
			shl		bx, 1		; 8086 shift left 3 time
			shl		bx, 1
			shl		bx, 1
			mov		cx, bx		; put result in cx
			shl		cx, 1		; 8086 shift left twice
			shl		cx, 1
			add		bx, cx		; add back into bx
			add		bx, ax		; add in column byte
			// line length fit in a single byte? (x1 % 8) + x2 - x1 < 8?
			mov		cx, x1		; load x1
			and		cx, 07h		; mod 8
			add		cx, x2
			sub		cx, x1

			cmp		cx, 08h
			jl		LEFT		; only plot left byte
			xor		es:[bx], dl	; plot left most byte
			mov		dl, 0FFh	; load a full byte horizontal line
			// line length overlap 2 bytes? (x1 % 8) + x2 - x1 < 16?
			cmp		cx, 10h
			jl		RIGHT		; plot right most byte as well
			shr		cx, 1		; divide by 8
			shr		cx, 1
			shr		cx, 1
			dec		cx
	FILL:	inc		bx			; point to next byte along
			xor		es:[bx], dl	; plot filler byte
			loop	FILL
			// construct the right most byte
	RIGHT:	inc		bx
	LEFT:	mov		cx, 07h		; load cx with ...0111
			sub		cx, x2		; subtract x2
			and		cl, 07h		; mask off 0111 lower bits(mod 8)
			mov		dh, 0FFh	; load dh with 1111111
			shl		dh, cl		; shift x2 mod 8
			and		dl, dh		; mask off dl
			// plot the right most byte
			xor		es:[bx], dl	; xor pixel bit in video buffer

	END:	pop		es
			pop		dx
			pop		cx
			pop		bx
			pop		ax
		}
	}

	void bline(int16_t x1, int16_t y1, int16_t x2, int16_t y2) {
		int16_t delta_x, delta_y, step_x, step_y, i1, i2;
		__asm {
			.8086
			push	ax
			push	bx
			push	cx
			push	dx
			push    di

			// calculate deltas, steps and distance limits
			mov		bx, x1		; load starting point x
			mov		ax, y1		; load starting point y
			// is it a positive or negative x direction?
			mov		step_x, -1	; assume -ve x direction
			sub		bx, x2		; calculate delta x
			jge		NEGX		; yes -ve x direction so calculate y direction
			neg		bx			; abs(delta x)
			mov		step_x,	1	; +ve x direction
			// is it a positive or negative y direction?
	NEGX:	mov		delta_x, bx	; store delta x
			mov		step_y, -1	; assume -ve y direction
			sub		ax, y2		; calculate delta y
			jge		NEGY		; yes -ve y direction
			neg		ax			; abs(delta y)
			mov		step_y, 1	; +ve y direction
	NEGY:	mov		delta_y, bx	; store delta y
			// delta x in bx, delta y in ax
			inc		bx
			mov		cx, bx		; save x repeat count
			shr		bx, 1		; i1 = -delta_y + (delta_x + 1) div 2
			sub		bx, ax
			mov		i1, bx		; store i1
			inc		ax
			mov		dx, ax		; save y repeat count
			shr		ax, 1		; i2 = delta_x - (delta_y + 1) div 2
			sub		ax, cx		; adjust possible repeate count
			inc		ax			
			neg		ax
			mov		i2, ax		; store i1
			cmp		cx, dx		; repeat count max(delta_x + 1, delta_y + 1)
			jge		BIGX		; delta_x is bigger
			mov		cx, dx		; delta_y is bigger
			// assume distance limit decision variable D = 0 and x,y start point
	BIGX:	mov		bx, x1
			mov		ax, y1
			mov		di, 0		; zero D
			jmp		BPLOT		; plot the first point
	MORE:	mov		dx, di		; load decision variable D
			cmp		dx, i1
			jl		HZ			; D too -ve so must be horizontal
			add		ax, step_y	; vertical step
			sub		dx, delta_x ; update decision variable D - delta_x
			cmp		di, i2		; check old D diagonal move
			jg		NDIAG		; no horizontal only
	HZ:		add		bx, step_x	; x = x + step_x
			add		dx, delta_y ; update decision variable D + delta_y
	NDIAG:	mov		di, dx		; store D
			// inline plot pixel (x,y) as (bx,ax)
	BPLOT:	push	ax
			push	bx
			push	cx
			push	dx
			push    es

			// ax column byte, bx 80 byte row, dl pixel bit 
			mov		cx, EVEN_LINES	
			//mov		bx, y		; load y
			test	bx, 01h		; is it an odd row?
			jz		EVEN		; no keep even lines offset
			mov		cx, ODD_LINES	
	EVEN:	mov		es, cx		; offset into extended segment
			// mode 6 is 1 bit per pixel
			// bit to set within column byte is x mod 8
			//mov		ax, x		; load x
			mov		cx, ax		; copy x
			and		cx, 07h		; mask off 0111 lower bits (mod 8)
			mov		dl, 080h	; load dl with 10000000
			shr		dl, cl		; shift single bit along by x mod 8
			// column byte is x/8
			shr		ax, 1		; 8086 shift right 3 times
			shr		ax, 1
			shr		ax, 1
			// row	= y/2 * 80 bytes per row
			//		= y * 40
			//		= y * 0x28
			//		= y * 101000 = 3 shl, add, 2 shl, add
			and		bx, 0FFFEh	; remove even / odd row bit from y
			shl		bx, 1		; 8086 shift left 3 time
			shl		bx, 1
			shl		bx, 1
			mov		cx, bx		; put result in cx
			shl		cx, 1		; 8086 shift left twice
			shl		cx, 1
			add		bx, cx		; add back into bx
			add		bx, ax		; add in column byte
			or		es:[bx], dl	; set pixel bit in video buffer

			pop		es
			pop		dx
			pop		cx
			pop		bx
			pop		ax

			loop	MORE

			pop		di
			pop		dx
			pop		cx
			pop		bx
			pop		ax
		}
	}

	//void box(uint16_t x, uint16_t y, uint16_t w, uint16_t h) {

	//}

}

/* plot core
					mov		ax, 0B800h	
					mov		bx, y 
					test	bx, 01h 
					jz		EVEN 
					mov		ax, 0BA00h 
			EVEN :	mov		es, ax
					mov		ax, x 
					mov		cx, ax 
					and		cx, 07h 
					mov		dl, 080h 
					shr		dl, cl 
					shr		ax, 1 
					shr		ax, 1 
					shr		ax, 1 
					and		bx, 0FFFEh 
					shl		bx, 1 
					shl		bx, 1 
					shl		bx, 1 
					mov		cx, bx
					shl		cx, 1 
					shl		cx, 1 
					add		bx, cx 
					add		bx, ax 
					or es: [bx] , dl
*/
