#include "equipment.h"

namespace dos {

	equipment& equipment::instance() {
		static equipment e;
		return e;
	}

}
