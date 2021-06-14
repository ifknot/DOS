#pragma once

namespace system {

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

}
