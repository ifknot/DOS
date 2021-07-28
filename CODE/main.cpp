#include <iostream>
#include <iomanip>
#include <cstdio>

#define MODE 1

#include "direct_system.h"
#include "direct_mode6.h"
//#include "direct_mode4.h"


int main() {

    std::cout << "*** test harness ***\n\n";
    dos::video_state_t old = system::get_video_state();
    std::cout << "press enter...";
    //-----------------------------------------------------
    
    std::getchar();
    system::set_video_mode(dos::GRAPHICS_MONOCHROME_640X200);

    uint32_t t1, t2;
    system::reset_clock_counter(0);
    int redraws = 50;

    std::cout << redraws << " circle redraws = ";
    t1 = system::read_clock_counter();
    for (int i = 0; i < redraws; ++i) {
        int16_t x = 25 - (i * 5);
        mode6::bresenham_circle<mode6::plot_or, mode6::clip_torus, mode6::scale_none>(x, 100, 50);
        mode6::bresenham_circle<mode6::plot_xor, mode6::clip_torus, mode6::scale_none>(x, 100, 50);
    }
    t2 = system::read_clock_counter();
    std::cout << ((float)(t2 - t1)) / 18.206 << "sec\n";

    std::cout << redraws << " scaled circle redraws = ";
    t1 = system::read_clock_counter();
    for (int i = 0; i < redraws; ++i) {
        int16_t y = 50 - (i * 5);
        mode6::bresenham_circle<mode6::plot_or, mode6::clip_torus, mode6::scale_third>(320, y, 50);
        mode6::bresenham_circle<mode6::plot_xor, mode6::clip_torus, mode6::scale_third>(320, y, 50);
    }
    t2 = system::read_clock_counter();
    std::cout << ((float)(t2 - t1)) / 18.206 << "sec\n";

    std::cout << redraws << " npx scaled circle redraws = ";
    t1 = system::read_clock_counter();
    for (int i = 0; i < redraws; ++i) {
        mode6::bresenham_circle<mode6::plot_or, mode6::clip_torus, mode6::scale_npx>(-5 + (i * 5), 640 + (i * 5), 50);
        mode6::bresenham_circle<mode6::plot_xor, mode6::clip_torus, mode6::scale_npx>(-5 + (i * 5), 640 + (i * 5), 50);
    }
    t2 = system::read_clock_counter();
    std::cout << ((float)(t2 - t1)) / 18.206 << "sec\n";

    //-----------------------------------------------------
    std::cout << "press enter...";
    std::getchar();
    system::set_video_mode(old.mode);
    std::cout << "\nOK\n";
    return 0;

}

/*

    std::getchar();
    system::set_video_mode(dos::GRAPHICS_MONOCHROME_640X200);

    uint32_t t1, t2;
    system::reset_clock_counter(0);
    int redraws = 10;

    std::cout << redraws << " circle redraws = ";
    t1 = system::read_clock_counter();
    for (int i = 0; i < redraws; ++i) {
        mode6::bresenham_circle(200, 100, 50);
    }
    t2 = system::read_clock_counter();
    std::cout << ((float)(t2 - t1)) / 18.206 << "sec\n";

    std::cout << redraws << " scaled circle redraws = ";
    t1 = system::read_clock_counter();
    for (int i = 0; i < redraws; ++i) {
        mode6_scaled::bresenham_circle(320, 300, 50);
    }
    t2 = system::read_clock_counter();
    std::cout << ((float)(t2 - t1)) / 18.206 << "sec\n";

    std::cout << redraws << " box redraws = ";
    t1 = system::read_clock_counter();
    for (int i = 0; i < redraws; ++i) {
        mode6::fast_horizontal_line(150, 250, 50);
        mode6::fast_horizontal_line(150, 250, 150);
        mode6::fast_vertical_line(150, 50, 150);
        mode6::fast_vertical_line(250, 50, 150);
    }
    t2 = system::read_clock_counter();
    std::cout << ((float)(t2 - t1)) / 18.206 << "sec\n";

    std::cout << redraws << " scaled box redraws = ";
    t1 = system::read_clock_counter();
    for (int i = 0; i < redraws; ++i) {
        mode6_scaled ::fast_horizontal_line(270, 370, 250);
        mode6_scaled::fast_horizontal_line(270, 370, 350);
        mode6_scaled::fast_vertical_line(270, 250, 350);
        mode6_scaled::fast_vertical_line(370, 250, 350);
    }
    t2 = system::read_clock_counter();
    std::cout << ((float)(t2 - t1)) / 18.206 << "sec\n";

    std::cout << redraws << " line redraws = ";
    t1 = system::read_clock_counter();
    for (int i = 0; i < redraws; ++i) {
        mode6::bresenham_line(150, 50, 250, 150);
        mode6::bresenham_line(250, 50, 150, 150);
    }
    t2 = system::read_clock_counter();
    std::cout << ((float)(t2 - t1)) / 18.206 << "sec\n";

    std::cout << redraws << " scaled line redraws = ";
    t1 = system::read_clock_counter();
    for (int i = 0; i < redraws; ++i) {
        mode6_scaled::bresenham_line(270, 250, 370, 350);
        mode6_scaled::bresenham_line(370, 250, 270, 350);
    }
    t2 = system::read_clock_counter();
    std::cout << ((float)(t2 - t1)) / 18.206 << "sec\n";

*/

/*

        0x55, 0x00, 0xAA, 0x00, 0xFF, 0x00,
        0x00, 0x55, 0x00, 0xAA, 0x00, 0xFF,
        0x55, 0x00, 0xAA, 0x00, 0xFF, 0x00,
        0x00, 0x55, 0x00, 0xAA, 0x00, 0xFF,
        0x55, 0x00, 0xAA, 0x00, 0xFF, 0x00,
        0x00, 0x55, 0x00, 0xAA, 0x00, 0xFF

system::set_video_mode(dos::GRAPHICS_4_COLOUR_300x200);

    std::getchar();

    cga::point_t p(100,100);
    cga::dim_t d(6,6); // 48x48

    uint8_t bitmap[36] = {
        0x55, 0x00, 0xAA, 0x00, 0xFF, 0x00,
        0x00, 0x55, 0x00, 0xAA, 0x00, 0xFF,
        0x55, 0x00, 0xAA, 0x00, 0xFF, 0x00,
        0x00, 0x55, 0x00, 0xAA, 0x00, 0xFF,
        0x55, 0x00, 0xAA, 0x00, 0xFF, 0x00,
        0x00, 0x55, 0x00, 0xAA, 0x00, 0xFF
    };

    mode4::paste(p.x, p.y, d.width / 2, d.height, bitmap);

/*

#include "random.h"

void cbox(uint16_t x, uint16_t y, uint16_t w) {
    //mode6::hline(x, x + w, y);
    //mode6::hline(x, x + w, y + w);
    //mode6::vline(x, y, y + w);
    //mode6::vline(x + w, y, y + w);
}

void slo_vline(uint16_t x, uint16_t y1, uint16_t y2) {
    for (uint16_t i = 0; i <= y2 - y1; ++i) {
        mode4::plot(x, y1 + i, cga::cyan);
    }
}

void slo_hline(uint16_t x1, uint16_t x2, uint16_t y) {
    for (uint16_t i = 0; i <= x2 - x1; ++i) {
        mode4::xor_plot(x1 + i, y, cga::magenta);
    }
}

void slo_box(uint16_t x, uint16_t y, uint16_t w) {
    slo_hline(x, x + w, y);
    slo_hline(x, x + w, y + w);
    slo_vline(x, y, y + w);
    slo_vline(x + w, y, y + w);
}

*/

/*

uint32_t t1, t2;

    std::cout << "*** test harness ***\n\n";

    system::reset_clock_counter(100);

    std::cout << std::dec << system::read_clock_counter() << '\n';

    t1 = system::read_clock_counter();

    std::cout << "detect npx = " << (system::detect_8087() ? "yes" : "no") << '\n';

    std::cout << "detect crtc = " << (system::detect_crtc() ? "yes" : "no") << '\n';

    std::cout << "display adapter = " << dos::video_adapter_names[system::get_video_adapter_type()] << '\n';

    dos::video_state_t v = system::get_video_state();

    std::cout << std::dec << (int)v.columns << " columns mode " << v.mode << std::hex << " page " << (int)v.page << '\n';

    std::cout << std::dec  << system::read_clock_counter() - t1 << '\n';

    std::getchar();

    //system::set_video_mode(dos::GRAPHICS_MONOCHROME_640X200);
    system::set_video_mode(dos::GRAPHICS_4_COLOUR_300x200);

    dos::video_state_t old = v;
    v = system::get_video_state();

    //std::cout << std::dec << (int)v.columns << " columns mode " << v.mode << std::hex << " page " << (int)v.page;

    std::getchar();

    
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
    

t1 = system::read_clock_counter();
for (uint16_t j = 1; j < 10; j += 2) {
    for (uint16_t i = 0; i < 189; i += 11) {
        //if (i % 2 == 0) {
        slo_box(i, i, j);
        //}
    }
}
t2 = system::read_clock_counter();
std::cout << t2 - t1 << '\n';

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

std::cout << std::dec << (int)v.columns << " columns mode " << v.mode << std::hex << " page " << (int)v.page << '\n';

std::cout << std::dec << system::read_clock_counter() << '\n';

*/

/*



*/
