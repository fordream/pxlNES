#include <iostream>
#include "CPU.h"
#include "IO.h"

int main(int argc, char* argv[]) {
    IO_load_iNES("smb.nes");
    CPU_init();
    PPU_init();
    CPU_run();

    CPU_dispose();
    PPU_dispose();

    return 0;
}
