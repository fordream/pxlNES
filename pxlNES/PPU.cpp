#include "PPU.h"

int8 VRAM[VRAM_EMULATED_SIZE];
int8 OAM[OAM_SIZE];
int8 PPU_registers[8];

std::thread PPU_thread;
bool PPU_thread_exit = false;

void PPU_run() {
    while (PPU_thread_exit) {

    }
}

void PPU_init() {
    for (int n = 0; n < 8; ++n) {
        PPU_registers[n] = 0;
    }

    PPU_thread = std::thread(PPU_run);
    PPU_thread.join();
}

void PPU_dispose() {
    PPU_thread_exit = true;
    PPU_thread.detach();
    delete[] VRAM;
    delete[] OAM;
}