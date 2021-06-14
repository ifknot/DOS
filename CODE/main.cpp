#include <iostream>
#include <iomanip>

#include "direct_system.h"

int main() {

    std::cout << "*** test harness ***\n\n";
    
    std::cout << "detect npx = " << (system::detect_8087() ?"yes" :"no") << '\n';

    std::cout << "detect crtc = " << (system::detect_crtc() ? "yes" : "no") << '\n';

    std::cout << "display adapter = " << dos::video_adapter_names[system::get_video_adapter_type()] << '\n';



    system::set_video_mode(dos::GRAPHICS_MONOCHROME_640X200);

    dos::video_state_t v = system::get_video_state();

    std::cout << (int)v.columns << " columns mode " << v.mode << std::hex << " page " << (int)v.page;
   
    std::cout << "\nOK\n";
    return 0;

}
