#include "test_display.h"

#include <iostream>
#include <cstdio>

bool test_display() {
	std::cout << "*** test display ***\n";
	dos::display& display = dos::display::instance();
	std::cout << dos::adapter_strings[display.detect_adapter()] << '\n'
		<< "crtc " << ((display.detect_crtc()) ?"yes" :"not") << " found\n";
	dos::video_state_t old = display.video_state();
	for (int m = 0; m < 16; ++m) {
		std::getchar();
		display.mode((dos::video_mode_t)m);
		dos::video_state_t v = display.video_state();
		std::cout << std::dec << (int)v.columns << ' ' << std::hex << (int)v.mode << ' ' << (int)v.page << '\n';
	}
	std::getchar();
	display.mode(old.mode);
	return true;
}
