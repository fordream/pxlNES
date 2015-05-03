#include "IO.h"
#include "CPU.h"

void IO_load_iNES(std::string path) {
    std::ifstream file;
    file.open(path, std::ios::ate | std::ios::binary);
    int32 size = file.tellg();
    file.seekg(0);

    if (size >= INES_HEADER_LEN) {
        std::string str(size, ' ');
        file.read(&str[0], size);

        if (str[0] == 'N' && str[1] == 'E' && str[2] == 'S' && str[3] == SUB_CHAR) {
            num_PRGs = str[4];
            num_CHRs = str[5];
            int8 flag6 = str[6];
            bool hmirror = ~flag6 & 1;
            bool SRAM_enabled = flag6 & 2;
            bool has_bp_PRG_RAM = flag6 & 4; //if true, cartridge has 4kb RAM at PPU $2000-$2FFF
            bool VRAM_4screen = flag6 & 8;

            prg_data = str.substr(INES_HEADER_LEN, str.length());
        }
    }
}