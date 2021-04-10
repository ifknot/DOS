#include "display.h"

#include <dos.h>
#include <iostream>

namespace dos {

	display& display::instance() {
		static display d;
		return d;
	}

	void display::mode(video_mode_t m) {
		union REGS r;
		r.h.ah = SET_VIDEO_MODE;
		r.h.al = m;
		int86(0x10, &r, &r);
	}

	video_state_t display::video_state() {
		video_state_t v;
		union REGS r;
		r.h.ah = GET_VIDEO_STATE;
		int86(0x10, &r, &r);
		v.columns = r.h.ah;
		v.mode = (video_mode_t)r.h.al;
		v.page = r.h.bh;
		return v;
	}

	bool display::detect_crtc() {
		char found = 1;
		__asm {
				.8086
				push	ax
				push	cx
				push	dx
				mov     al, 0Fh
				out     dx, al		; select 6845 reg 0Fh(Cursor Low)
				inc     dx
				in      al, dx		; AL: = current Cursor Low value
				mov     ah, al		; preserve in AH
				mov     al, 66h		; AL: = arbitrary value
				out     dx, al		; try to write to 6845

				mov     cx, 100h
		WAIT:	loop    WAIT		; wait for 6845 to respond

				in      al, dx
				xchg    ah, al
				out     dx, al		; restore original value

				cmp     ah, 66h		; test whether 6845 responded
				je      YES			; jump if it did(cf is reset)

				mov 	found, 0	; clear the found flag

		YES:	pop		dx
				pop		cx
				pop		ax

		}
		return (bool)found;
	}

	/**
	 * Set AX=1200h, BL=32h and call INT 10.
	 * If AL returns 12h, you have a VGA.
	 * If not, set AH=12h, BL=10h and call INT 10 again.
	 * If BL returns 0,1,2,3, you have an EGA with 64,128,192,256K memory.
	 * If not, set AH=0Fh and call INT 10 a third time.
	 * If AL is 7, you have an MDA (original monochrome adapter) or Hercules;
	 * if not, you have a CGA..
	 *
	 * \return
	 */
	video_adapter_t display::detect_adapter() {
		union REGS reg;
		// detect VGA
		reg.h.ah = VIDEO_SUB_CONFIG;
		reg.h.bl = ACCESS_VIDEO_RAM;
		reg.h.al = 0x00;	//enable CPU access to video RAM and I/O ports
		int86(0x10, &reg, &reg);
		if (reg.h.al == 0x12) {
			return VGA;
		}
		// detect EGA
		reg.h.ah = VIDEO_SUB_CONFIG;
		reg.h.bl = CONFIG_INFO;
		int86(0x10, &reg, &reg);
		if (reg.h.bl != CONFIG_INFO) {
			return EGA;
		}
		// detect MDA
		reg.h.ah = GET_VIDEO_STATE;
		int86(0x10, &reg, &reg);
		if (reg.h.al == TEXT_MONOCHROME_80X25) {
			return MDA;
		}
		return CGA;
	}

	video_adapter_info_t display::adapter_info() {
		return video_adapter_info_t();
	}

	

	

}
