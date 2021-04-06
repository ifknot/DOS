#pragma once

#include <stdint.h>
#include <vector>

#include "display_types.h"

namespace dos {

    /**
    * C98 Singleton pattern
    */
    class display {

    public:

        static display& instance();

        void mode(video_mode_t m);

        video_state_t mode();

        std::vector<bool>& modes();


    private:

        std::vector<bool> modes_;

        display() {}
        display(display const&);
        void operator=(display const&);

    };

}