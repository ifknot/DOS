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

	video_state_t display::mode() {
		video_state_t v;
		union REGS r;
		r.h.ah = GET_VIDEO_STATE;
		int86(0x10, &r, &r);
		v.columns = r.h.ah;
		v.mode = (video_mode_t)r.h.al;
		v.page = r.h.bh;
		return v;
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
	video_adapter_t display::adapter() {
		union REGS r;
		r.h.ah = VIDEO_SUB_CONFIG;
		r.h.bl = CONFIG_INFO;
		int86(0x10, &r, &r);
		if (r.h.bl == CONFIG_INFO) {
			return CGA;
		}
		return OTHER;
	}

	

	

}
