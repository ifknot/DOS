#include <iostream>
#include <iomanip>

#include "direct_system.h"

int main() {

    std::cout << "*** test harness ***\n\n";
    
    std::cout << "detect npx = " << (system::detect_8087() ?"yes" :"no") << '\n';

    std::cout << "detect crtc = " << (system::detect_crtc() ? "yes" : "no") << '\n';

    std::cout << "detect video = " << dos::video_adapter_names[system::get_video_adapter_type()] << '\n';
   
    std::cout << "OK\n";
    return 0;

}
