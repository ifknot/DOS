#include "display.h"

namespace xt {



	display& display::instance() {
		static display d;
		return d;
	}

}
