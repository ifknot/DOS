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
   
    //std::getchar();

    //mode6::plot2(0, 0);
    //mode6::plot2(1, 1);
    //mode6::plot2(2, 2);
    //mode6::plot2(4, 3);
    //mode6::plot2(5, 4);
    //mode6::plot2(6, 5);
    //mode6::plot2(7, 8);
    //mode6::plot2(8, 9);
    //mode6::plot2(100, 10);
    
    for (int i = 0; i < 100; ++i) {
        mode6::plot2(i, i);
    }

    std::getchar();

    system::set_video_mode(old.mode);

    v = system::get_video_state();

    std::cout << std::dec << (int)v.columns << " columns mode " << v.mode << std::hex << " page " << (int)v.page;

    std::cout << "\nOK\n";
    return 0;

}
