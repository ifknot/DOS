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

        video_state_t video_state();

        /**
         * Detects the presence of the 6845 CRTC chip on a MDA, CGA or HGC.
         * The technique is to write and read register 0Fh of the chip (cursor low).  
         * If the same value is read as written, assume the chip is present at the specified port addr.
         * @note not all IBM cards used the same Motorola MC6845 chip. Some used a Hitachi HD68456845 CRTC chip on a MDA, CGA or HGC.
         * @return true if detected, otherwise false
         */
        bool detect_crtc();

        video_adapter_t detect_adapter();

        video_adapter_info_t info();

    private:

        display() {}
        display(display const&);
        void operator=(display const&) {}

    };

}