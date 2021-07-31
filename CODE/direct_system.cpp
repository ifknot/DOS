#include "direct_system.h"

#include <stdint.h>
#include <dos.h>

namespace system {

	using namespace dos;

	bool detect_8087() {
        uint16_t status = 0xFF;
        __asm {
            .8086
            .8087
            push	cx

            fninit				; no wait initialise the NPX(if there is one)
            mov		cx, 64h		; idle in an empty loop whilst fninit loads the status word
    L1:		loop	L1
            fnstsw	status		; save the status word which will reset the lower byte to 0
            mov		cx, 64h		; idle in an empty loop whilst fnstsw stores the status word
	L2:		loop	L2

            pop		cx
        }
        return status == 0;
	}

    bool detect_crtc() {
		uint8_t found = 1;
		__asm {
			.8086
			push	ax
			push	cx
			push	dx

			mov		dx, 3DAh
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

	dos::video_adapter_t get_video_adapter_type() {
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

	void set_video_mode(video_mode_t mode) {
		union REGS r;
		r.h.ah = SET_VIDEO_MODE;
		r.h.al = mode;
		int86(0x10, &r, &r);
	}

	video_state_t get_video_state() {
		video_state_t v;
		union REGS r;
		r.h.ah = GET_VIDEO_STATE;
		int86(0x10, &r, &r);
		v.columns = r.h.ah;
		v.mode = (video_mode_t)r.h.al;
		v.page = r.h.bh;
		return v;
	}

	uint32_t read_clock_counter() {
		uint32_t c;
		union REGS r;
		r.h.ah = 0;
		int86(0x1A, &r, &r);
		c = r.x.cx;
		c <<= 16;
		c |= r.x.dx;
		return c;
	}

	void reset_clock_counter(uint32_t t) {
		uint16_t lo = t;
		uint16_t hi = t >> 16;
		union REGS r;
		r.h.ah = 1;
		r.x.cx = hi;
		r.x.dx = lo;
		int86(0x1A, &r, &r);
	}

}
