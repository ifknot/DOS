#pragma once

#include <stdint.h>

#include "display_types.h"

namespace dos {

    /**
    * C98 Singleton pattern
    */
    class display {

    public:

        static display& instance();

        void mode(video_mode_t m);

        video_state_t state();

        video_adapter_t adapter();

    private:

        display() {}
        display(display const&);
        void operator=(display const&) {}

    };

}