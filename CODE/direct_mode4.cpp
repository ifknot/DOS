#include "direct_mode4.h"

#include <cassert>

#define EVEN_LINES	0B800h	// even lines video buffer memory	
#define ODD_LINES	0BA00h	// odd lines video buffer memory

namespace mode4 {

	void plot(uint16_t x, uint16_t y, cga::colour_t colour) {
		assert(x < 320 && y < 200);
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
			test	bx, 01h		; is it an odd row ?
			jz		EVEN		; no keep even lines offset
			mov		ax, ODD_LINES
	EVEN:	mov		es, ax		; offset into extended segment
			// mode 4 is 2 bits per pixel
			// bit to set within column byte is x mod 4
			mov		ax, x		; load x
			mov		cx, ax		; copy x
			and		cx, 03h		; mask off 0011 lower bits(x mod 4)
			shl		cx, 1		; x 2 (2 bits of colour information)
			mov		dl,	colour	; load 2 bits of colour
			shr		dl, cl		; shift colour along by x mod 4
			// column byte is x/4
			shr		ax, 1		; 8086 shift right 2 times
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

	void xor_plot(uint16_t x, uint16_t y, cga::colour_t colour) {
		assert(x < 320 && y < 200);
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
	EVEN:	mov		es, ax
			mov		ax, x
			mov		cx, ax
			and		cx, 03h
			shl		cx, 1
			mov		dl, colour
			shr		dl, cl
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
			xor		es:[bx], dl

			pop es
			pop	dx
			pop	cx
			pop	bx
			pop	ax
		}
	}

	uint16_t paste(uint16_t add(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t data[], uint16_t size);) {
		uint16_t sum;
		__asm {
			.8086
			push ds
			push si
			push cx
			push ax

			lds		si, arr
			mov		cx, len
			cld 

			
			pop ax
			pop cx
			pop si
			pop ds
		}
		return sum;
	}

	

	

}
