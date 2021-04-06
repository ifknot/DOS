#include "display.h"

#include <dos.h>

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
		r.h.ah = GET_VIDEO_MODE;
		int86(0x10, &r, &r);
		v.columns = r.h.ah;
		v.mode = r.h.al;
		v.page = r.h.bh;
		return v;
	}

	std::vector<bool>& display::modes() {
		return modes_;
	}

}
