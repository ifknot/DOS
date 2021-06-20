#include "direct_mode6.h"

#include <cassert>

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
			mov		ax, 0B800h	; even lines video buffer memory
			mov		bx, y		; load y
			test	bx, 01h		; is it an odd row?
			jz		EVEN		; no keep even lines offset
			mov		ax, 0BA00h	; odd lines video buffer memory
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

			// ax column byte, bx 80 byte row, dl pixel bit 
			mov		ax, 0B800h	; even lines video buffer memory
			mov		bx, y		; load y
			test	bx, 01h		; is it an odd row?
			jz		EVEN		; no keep even lines offset
			mov		ax, 0BA00h	; odd lines video buffer memory
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
			xor		es:[bx], dl	; set pixel bit in video buffer

			pop es
			pop	dx
			pop	cx
			pop	bx
			pop	ax
		}
	}

	void vline(uint16_t x, uint16_t y1, uint16_t y2) {
		assert(x < 640 && y1 < 200 && y2 < 200 && y1 < y2);
		__asm {
			.8086
			push	ax
			push	bx
			push	cx
			push	dx
			push    es

			// starting page odd/even
			mov		ax, 0B800h	; even lines video buffer memory
			mov		bx, y1		; load y
			test	bx, 01h		; is it an odd row?
			jz		EVEN		; no keep even lines offset
			mov		ax, 0BA00h	; odd lines video buffer memory
	EVEN:	mov		es, ax		; offset into extended segment
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
			// plot vertical points
			mov		cx, y2		
			sub		cx, y1		; cx is line length
			jz		END			; y1 == y2 line is 1 pixel 
	PLOT:	mov		ax, es
			xor		ax, 200h	; swap to opposite row video buffer
			mov		es, ax
			or		es:[bx], dl ; set pixel bit in video buffer
			dec		cx
			jz		END
			add		bx, 50h		; increment bx to next row
			mov		ax, es
			xor		ax, 200h	; swap to opposite row video buffer
			mov		es, ax
			or		es:[bx], dl	; set pixel bit in video buffer
			loop	PLOT

	END:	pop es
			pop	dx
			pop	cx
			pop	bx
			pop	ax
		}
	}

	void xor_vline(uint16_t x, uint16_t y1, uint16_t y2) {
		assert(x < 640 && y1 < 200 && y2 < 200 && y1 < y2);
		__asm {
			.8086
			push	ax
			push	bx
			push	cx
			push	dx
			push    es

			// starting page odd/even
			mov		ax, 0B800h	; even lines video buffer memory
			mov		bx, y1		; load y
			test	bx, 01h		; is it an odd row?
			jz		EVEN		; no keep even lines offset
			mov		ax, 0BA00h	; odd lines video buffer memory
	EVEN:	mov		es, ax		; offset into extended segment
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
			xor		es:[bx], dl	; set pixel bit in video buffer
			// plot vertical points
			mov		cx, y2		
			sub		cx, y1		; cx is line length
			jz		END			; y1 == y2 line is 1 pixel 
	PLOT:	mov		ax, es
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
			loop	PLOT

	END:	pop es
			pop	dx
			pop	cx
			pop	bx
			pop	ax
		}
	}

	void hline(uint16_t x1, uint16_t x2, uint16_t y) {
		// load dl with FF
		// shr dl, cl
		// if x2 - x1 < 8 then 
		//		and off the right 
		//		jmp plot the final dl byte
		// else
		//
		assert(x1 < 640 && x2 < 640 && y < 200 && x1 < x2);
		__asm {
			.8086
			push	ax
			push	bx
			push	cx
			push	dx
			push    es

			mov		ax, 0B800h	; even lines video buffer memory
			mov		bx, y		; load y
			test	bx, 01h		; is it an odd row?
			jz		EVEN		; no keep even lines offset
			mov		ax, 0BA00h	; odd lines video buffer memory
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
			// figure out if fit in a single byte? (x1 % 8) + x2 - x1 < 8?
			mov		cx, x1		; load x1
			and		cx, 07h		; mod 8
			add		cx, x2
			sub		cx, x1

			cmp		cx, 08h
			jl		LEFT
			or		es:[bx], dl	; plot left most byte
			mov		dl, 0FFh	; load a full byte horizontal line
			// if x2 - x1 < 16 jmp plot right (the line fits inside 2 bytes)
			cmp		cx, 10h
			jl		RIGHT
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
}

