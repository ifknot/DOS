#pragma once

#include "cga_types.h"
#include "cga_colours.h"

namespace cga {

	class bitmap {

	public:

		byte_vector_t	odd;
		byte_vector_t	even;

		bitmap(dim_t d);

		void resize(dim_t d);

		void fill(colour_t c);

		void clear();

	private:

		dim_t			dim;

	};

}
