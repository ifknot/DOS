#include <iostream>
#include <iomanip>
#include <cstdio>

#include "direct_system.h"
#include "direct_mode6.h"

void box(uint16_t x, uint16_t y, uint16_t w) {
    mode6::hline(x, x + w, y);
    mode6::hline(x, x + w, y + w);
    mode6::vline(x, y, y + w);
    mode6::vline(x + w, y, y + w);
}

int main() {

    std::cout << "*** test harness ***\n\n";
    
    std::cout << "detect npx = " << (system::detect_8087() ?"yes" :"no") << '\n';

    std::cout << "detect crtc = " << (system::detect_crtc() ? "yes" : "no") << '\n';

    std::cout << "display adapter = " << dos::video_adapter_names[system::get_video_adapter_type()] << '\n';

    dos::video_state_t v = system::get_video_state();

    std::cout << std::dec << (int)v.columns << " columns mode " << v.mode << std::hex << " page " << (int)v.page;

    std::getchar();

    system::set_video_mode(dos::GRAPHICS_MONOCHROME_640X200);

    dos::video_state_t old = v;
    v = system::get_video_state();

    //std::cout << std::dec << (int)v.columns << " columns mode " << v.mode << std::hex << " page " << (int)v.page;
   
    std::getchar();
    
    /*
    for (int i = 0; i < 100; ++i) {
        mode6::plot(i, i);
    }

    std::getchar();

    for (int i = 0; i < 100; i += 2) {
        mode6::xor_plot(i, i);
    }
    */
   /*
    for (int i = 0; i < 640; i += 1) {
        mode6::vline(i, 0, 199);
    }

    for (int i = 0; i < 640; i += 2) {
        mode6::xor_vline(i, 0, 199);
    }
    */

    
    for (uint16_t i = 0; i < 10; ++i) {
        //box(i, i, (i + 1) * 10);
    }

    box(1, 1, 11);
    box(14, 14, 22);



    std::getchar();

    system::set_video_mode(old.mode);

    v = system::get_video_state();

    std::cout << std::dec << (int)v.columns << " columns mode " << v.mode << std::hex << " page " << (int)v.page;

    std::cout << "\nOK\n";
    return 0;

}
