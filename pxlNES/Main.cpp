#include <iostream>
#include <PXL.h>
#include "CPU.h"
#include "IO.h"

int main(int argc, char* argv[]) {
    PXL_Window window(800, 600, "NES emulator");
    PXL_init();

    IO_load_iNES("smb.nes");
    CPU_init();
    PPU_init();
    CPU_run();

    CPU_dispose();
    PPU_dispose();

    return 0;
}
