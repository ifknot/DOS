#include <iostream>
#include <iomanip>
#include <cstdio>

#include "direct_system.h"
#include "direct_mode6.h"

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

    mode6::plot(0, 9);
    mode6::hline(0, 1, 10);
    mode6::hline(0, 2, 11);
    mode6::hline(0, 3, 12);
    mode6::hline(0, 4, 13);
    mode6::hline(0, 5, 14);
    mode6::hline(0, 6, 15);
    mode6::hline(0, 7, 16);
    mode6::hline(0, 8, 17);
    mode6::hline(0, 9, 18);
    mode6::hline(0, 10, 19);
    mode6::hline(0, 11, 20);
    mode6::hline(0, 12, 21);
    mode6::hline(0, 13, 22);

   // mode6::hline(1, 9, 18);
    //mode6::hline(2, 10, 19);
    //mode6::hline(3, 11, 20);
    //mode6::hline(4, 12, 21);
    //mode6::hline(5, 13, 22);
    //mode6::hline(6, 14, 23);
    //mode6::hline(7, 15, 24);
    
    for (uint16_t i = 1; i < 100; ++i) {
        //mode6::hline(0, i, 24 + i);
    }


    std::getchar();

    system::set_video_mode(old.mode);

    v = system::get_video_state();

    std::cout << std::dec << (int)v.columns << " columns mode " << v.mode << std::hex << " page " << (int)v.page;

    std::cout << "\nOK\n";
    return 0;

}
