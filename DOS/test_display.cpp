#include "test_display.h"

#include <iostream>
#include <cstdio>

bool test_display() {
	std::cout << "** test display ***\n";
	dos::display& d = dos::display::instance();
	for (int m = 0; m < 16; ++m) {
		std::getchar();
		d.mode((dos::video_mode_t)m);
		dos::video_state_t v = d.mode();
		std::cout << (int)v.columns << ' ' << std::hex << (int)v.mode << ' ' << (int)v.page << '\n';
	}
	
	return true;
}
