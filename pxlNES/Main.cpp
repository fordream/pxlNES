#include <iostream>
#include <PXL.h>
#include "CPU.h"
#include "IO.h"

int main(int argc, char* argv[]) {
    IO_load_iNES("smb.nes");
    PPU_init();
    CPU_init();
    CPU_run();

    while (true) {

    }

    CPU_dispose();
    PPU_dispose();

    return 0;
}
