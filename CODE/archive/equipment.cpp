#include "equipment.h"

namespace dos {

	equipment& equipment::instance() {
		static equipment e;
		return e;
	}

	bool equipment::detect(equipment_flags_t flags) {
		return false;
	}

}
