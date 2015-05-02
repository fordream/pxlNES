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
    IMPLIED, 
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
    INDIRECT_X, 
    INDIRECT_Y
};

struct OpCode {

    int8 bytes;
    int8 cycles;
};

int8* pre_op_list = new int8[] {
    0, BRK, IMPLIED,
    1, ORA, INDIRECT_X,
    5, ORA, ZERO_PAGE,
    6, ASL, ZERO_PAGE,
    8, PHP, IMPLIED,
    9, ORA, IMMEDIATE,
    10, ASL, ACCUMULATOR,
    13, ORA, ABSOLUTE,
    14, ASL, ABSOLUTE,
    16, BPL, IMPLIED,
    17, ORA, INDIRECT_Y,
    21, ORA, ZERO_PAGE_X, 
    22, ASL, ZERO_PAGE_X, 
    24, CLC, IMPLIED, 
    25, ORA, ABSOLUTE_Y, 
    29, ORA, ABSOLUTE_X, 
    30, ASL, ABSOLUTE_X, 
    32, JSR, IMPLIED, 
    33, AND, INDIRECT_X, 
    36, BIT, ZERO_PAGE, 
    37, AND, ZERO_PAGE, 
    38, ROL, ZERO_PAGE, 
    40, PLP, IMPLIED, 
    41, AND, IMMEDIATE, 
    42, ROL, ACCUMULATOR, 
    44, BIT, ABSOLUTE, 
    45, AND, ABSOLUTE, 
    46, ROL, ABSOLUTE, 
    48, BMI, IMPLIED, 
    49, AND, INDIRECT_Y, 
    53, AND, ZERO_PAGE_X, 
    54, ROL, ZERO_PAGE_X, 
    56, SEC, IMPLIED,
    57, AND, ABSOLUTE_Y, 
    61, AND, ABSOLUTE_X, 
    62, ROL, ABSOLUTE_X, 
    64, RTI, IMPLIED, 
    65, EOR, INDIRECT_X, 
    69, EOR, ZERO_PAGE, 
    70, LSR, ZERO_PAGE, 
    72, PHA, IMPLIED, 
    73, EOR, IMMEDIATE, 
    74, LSR, ACCUMULATOR, 
    76, JMP, ABSOLUTE, 
    77, EOR, ABSOLUTE, 
    78, LSR, ABSOLUTE, 
    80, BVC, IMPLIED, 
    81, EOR, INDIRECT_Y, 
    85, EOR, ZERO_PAGE_X
};

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
