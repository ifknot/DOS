#include "direct_mode6.h"

namespace mode6 {

	void plot(uint16_t x, uint16_t y) {
		__asm {
			.8086
			push	ax
			push	bx
			push	cx
			push	dx
			push    es

			mov		ax, 0B800h		; video buffer memory
			mov		es, ax			; extended segment
			mov		bx, x			; load x
			mov		ax, y			; load y
			mov		cx, bx			; copy x
			and		cx, 07h			; mask off 0111 lower bits 
			mov		dl, 080h		; load dl with 10000000
			shr		dl, cl			; shift single bit along 
			shr		bx, 1			; shift pixel position 
			shr		bx, 1			; out of 
			shr		bx, 1			; x coordinate
			push	ax				; save y
			and		ax, 0FFFEh		; remove even/odd row bit from y
			mov		cx, ax			; copy modified y
			shl		cx, 1			; offset of row in video memory
			shl		cx, 1			; is y / 2 * 50h (80 bytes in each row)
			add		cx, ax			; = y * 28h = y * 101000
			shl		cx, 1			; but shifts and adds are about
			shl		cx, 1			; 10 times faster than
			shl		cx, 1			; multiply
			add		bx, cx			; add to offset of column
			pop		ax				; recover y
			and		ax, 01h			; is it odd row?
			jz		PLOT			; if no plot
			add		bx, 2000h		; else add 2000h
	PLOT:	or		es:[bx], dl

			pop es
			pop	dx
			pop	cx
			pop	bx
			pop	ax
		}
	}

	void plot2(uint16_t x, uint16_t y) {
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
			mov		ax, x		
			mov		cx, ax		; copy x
			and		cx, 07h		; mask off 0111 lower bits
			mov		dl, 080h	; load dl with 10000000
			shr		dl, cl		; shift single bit along
			and		bx, 0FFFEh	; remove even / odd row bit from y
			// row	= y/2 * 80 bytes per row
			//		= y * 40
			//		= y * 0x28
			//		= y * 101000 = 3 shl, add, 2 shl, add
			shl		bx, 1
			shl		bx, 1
			shl		bx, 1
			mov		cx, bx
			shl		cx, 1
			shl		cx, 1
			add		bx, cx
			or		es:[bx], dl

			pop es
			pop	dx
			pop	cx
			pop	bx
			pop	ax
		}
	}

}
