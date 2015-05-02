#include <iostream>
#include <fstream>
#include <stdint.h>

//iNES header constants
#define INES_HEADER_LEN		    8			//min nes header valid byte len
#define SUB_CHAR				26			//used as escape sequence in MS-DOS

typedef char					int8;
typedef short					int16;
typedef int						int32;
typedef long long				int64;
typedef unsigned char			uint8;
typedef unsigned short			uint16;
typedef unsigned int			uint32;
typedef unsigned long long		uint64;

#define PRG_LEN 16384   //16kb
#define CHR_LEN 8192    //8kb

int16 num_PRGs;
int16 num_CHRs;

enum Instructions {
    ADC, AND, ASL, BCC, BCS, BEQ, BIT, BMI, BNE, BPL, BRK, BVC, BVS, CLC,
    CLD, CLI, CLV, CMP, CPX, CPY, DEC, DEX, DEY, EOR, INC, INX, INY, JMP,
    JSR, LDA, LDX, LDY, LSR, NOP, ORA, PHA, PHP, PLA, PLP, ROL, ROR, RTI,
    RTS, SBC, SEC, SED, SEI, STA, STX, STY, TAX, TAY, TSX, TXA, TXS, TYA
};

enum AddressMode {
    IMPLICIT,
    ACCUMULATOR,
    IMMEDIATE,
    ZERO_PAGE,
    ZERO_PAGE_X,
    ZERO_PAGE_Y,
    RELATIVE,
    ABSOLUTE,
    ABSOLUTE_X,
    ABSOLUTE_Y,
    INDIRECT,
    INDEXED_INDIRECT,
    INDIRECT_INDEXED
};

struct OpCode {

    int8 bytes;
    int8 cycles;
};

OpCode op_codes = 
{ "BRK" };

int main(int argc, char* argv[]) {
	std::ifstream file;
	file.open("smb.nes", std::ios::ate | std::ios::binary);
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

            std::string prg_data = str.substr(INES_HEADER_LEN, str.length());

            int a = 5;
		}
	}

	return 0;
}
