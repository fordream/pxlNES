#ifndef _PPU_H_
#define _PPU_H_

#include <thread>
#include "Constants.h"

extern int8 VRAM[VRAM_EMULATED_SIZE];
extern int8 OAM[OAM_SIZE];
extern int8 PPU_registers[8];

extern std::thread PPU_thread;

extern void PPU_run();
extern void PPU_init();
extern void PPU_dispose();

#endif