#ifndef _PPU_H_
#define _PPU_H_

#include <thread>
#include <PXL.h>
#include "Constants.h"

extern int8 VRAM[VRAM_EMULATED_SIZE];
extern int8 OAM[OAM_SIZE];
extern int8 PPU_registers[8];

extern std::thread PPU_thread;

extern PXL_Window PPU_display;

extern void PPU_run();
extern void PPU_init();
extern void PPU_dispose();

class DebugPrinter {

	public:
		std::ostringstream msg;

		DebugPrinter() { }

		template <class T> DebugPrinter &operator<<(const T &v) {
			msg << v;
			return *this;
		}

        ~DebugPrinter();
};

#define PPU_debug_write DebugPrinter()

#endif