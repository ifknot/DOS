#include <iostream>
#include <iomanip>
#include <cstdio>


#include "direct_system.h"
#include "direct_mode6.h"
#include "random.h"

void cbox(uint16_t x, uint16_t y, uint16_t w) {
    mode6::hline(x, x + w, y);
    mode6::hline(x, x + w, y + w);
    mode6::vline(x, y, y + w);
    mode6::vline(x + w, y, y + w);
}

void slo_vline(uint16_t x, uint16_t y1, uint16_t y2) {
    for (uint16_t i = 0; i <= y2 - y1; ++i) {
        mode6::plot(x, y1 + i);
    }
}

void slo_hline(uint16_t x1, uint16_t x2, uint16_t y) {
    for (uint16_t i = 0; i <= x2 - x1; ++i) {
        mode6::plot(x1 + i, y);
    }
}

void slo_box(uint16_t x, uint16_t y, uint16_t w) {
    slo_hline(x, x + w, y);
    slo_hline(x, x + w, y + w);
    slo_vline(x, y, y + w);
    slo_vline(x + w, y, y + w);
}

int main() {

    std::cout << "*** test harness ***\n\n";

    std::cout << std::dec << system::read_clock_counter() << '\n';

    uint32_t t1 = system::read_clock_counter();

    std::cout << "detect npx = " << (system::detect_8087() ? "yes" : "no") << '\n';

    std::cout << "detect crtc = " << (system::detect_crtc() ? "yes" : "no") << '\n';

    std::cout << "display adapter = " << dos::video_adapter_names[system::get_video_adapter_type()] << '\n';

    dos::video_state_t v = system::get_video_state();

    std::cout << std::dec << (int)v.columns << " columns mode " << v.mode << std::hex << " page " << (int)v.page << '\n';

    std::cout << std::dec  << system::read_clock_counter() - t1 << '\n';

    std::getchar();

    system::set_video_mode(dos::GRAPHICS_MONOCHROME_640X200);

    dos::video_state_t old = v;
    v = system::get_video_state();

    //std::cout << std::dec << (int)v.columns << " columns mode " << v.mode << std::hex << " page " << (int)v.page;
   
    std::getchar();
    /*
    t1 = system::read_clock_counter();
    for (int x = 0; x < 450; x += 25) {
        for (uint16_t j = 1; j < 10; j += 2) {
            for (uint16_t i = 0; i < 189; i += 11) {
                //if (i % 2 == 0) {
                cbox(i + x, i, j);
                //}
            }
        }
    }
    uint32_t t2 = system::read_clock_counter();
    std::cout << "\n\n\n" << ((float)(t2 - t1)) / 18.206 << "sec\n";
    */
    /*
    t1 = system::read_clock_counter();
    for (uint16_t j = 1; j < 10; j += 2) {
        for (uint16_t i = 0; i < 189; i += 11) {
            //if (i % 2 == 0) {
            slo_box(i + 55, i, j);
            //}
        }
    }
    t2 = system::read_clock_counter();
    std::cout << t2 - t1 << '\n';
    */
    //mode6::bline(0, 0, 199, 199);
    //mode6::bline(0, 0, 0, 199);
    //mode6::bline(0, 0, 25, 199);
    //mode6::bline(0, 0, 50, 199);
    //mode6::bline(0, 0, 199, 0);
    //mode6::bline(0, 199, 639, 0);
    
    t1 = system::read_clock_counter();
    for (int i = 0; i < 500; ++i) {
        mode6::bline(320, 100, random::xorshift32() % 640, random::xorshift32() % 200);
    }
    uint32_t t2 = system::read_clock_counter();
    std::cout << "\n\n\n" << ((float)(t2 - t1)) / 18.206 << "sec\n";

    std::getchar();

    system::set_video_mode(old.mode);

    v = system::get_video_state();

    std::cout << std::dec << (int)v.columns << " columns mode " << v.mode << std::hex << " page " << (int)v.page;

    std::cout << "\nOK\n";
    return 0;

}
