#pragma once

#include "display_types.h"

namespace system {

	using namespace dos;

	/**
	 * @brief Detect the presence of the Intel 8087 Numeric Processor Extension (NPX).
	 * @note NB 1 The fwait opcode 9Bh occurs in all but 2 of the NPX instructions
	 * fwait stops the CPu from procesing anymore instructions until its TEST pin is active and,
	 * since this pin is connected to the BUSY pin of the NPX, the CPU will not send the NPX any
	 * instructions until it has finished with witht he previous one.
	 * @note NB 2 The assembler automatically inserts the fwait opcode unless it is prevented
	 * from doing so by the presence of 'n' for - no wait - in the instruction mnemonic.
	 * @return bool - true if NPX detected, otherwise false
	 */
	bool detect_8087();

	/**
	 * Detects the presence of the 6845 CRTC chip on a MDA, CGA or HGC.
	 * The technique is to write and read register 0Fh of the chip (cursor low).
	 * If the same value is read as written, assume the chip is present at the specified port addr.
	 * @note not all IBM cards used the same Motorola MC6845 chip. Some used a Hitachi HD68456845 CRTC chip on a MDA, CGA or HGC.
	 * @return true if detected, otherwise false
	 */
	bool detect_crtc();

	/**
	 * Set AX=1200h, BL=32h and call INT 10.
	 * If AL returns 12h, you have a VGA.
	 * If not, set AH=12h, BL=10h and call INT 10 again.
	 * If BL returns 0,1,2,3, you have an EGA with 64,128,192,256K memory.
	 * If not, set AH=0Fh and call INT 10 a third time.
	 * If AL is 7, you have an MDA (original monochrome adapter) or Hercules;
	 * if not, you have a CGA..
	 *
	 * @return enum video_adapter_t
	 */
	dos::video_adapter_t get_video_adapter_type();

	/**
	 * .
	 * 
	 * @param mode
	 */
	void set_video_mode(video_mode_t mode);

	/**
	 * .
	 * @return struct video_state_t
	 */
	video_state_t get_video_state();

	/**
	 * INT 1A,0 - Read System Clock Counter
	 * AH = 00
	 * on return:
	 * AL = midnight flag, 1 if 24 hours passed since reset
	 * CX = high order word of tick count
	 * DX = low order word of tick count
	 * - incremented approximately 18.206 times per second
	 * - at midnight CX:DX is zero
	 * - this function can be called in a program to assure the date is updated after midnight
	 * - this will avoid the passing two midnights date problem.
	 */
	uint32_t read_clock_counter();

	//void reset_clock_counter();

}
