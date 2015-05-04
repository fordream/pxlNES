#include "PPU.h"

int8 VRAM[VRAM_EMULATED_SIZE];
int8 OAM[OAM_SIZE];

std::thread PPU_thread;
bool PPU_thread_exit = false;

void PPU_run() {
    while (PPU_thread_exit) {

    }
}

void PPU_init() {
    PPU_thread = std::thread(PPU_run);
    PPU_thread.join();
}

void PPU_dispose() {
    PPU_thread_exit = true;
    PPU_thread.detach();
    delete[] VRAM;
    delete[] OAM;
}