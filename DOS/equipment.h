#pragma once

#include <stdint.h>

#include "equipment_types.h"

namespace dos {

    /**
    * C98 Singleton pattern
    */
    class equipment {

    public:

        static equipment& instance();

    private:

        equipment() {}
        equipment(equipment const&);
        void operator=(equipment const&) {}

    };

}