#include <iostream>
#include <fstream>
#include <stdint.h>

//iNES header constants
#define INES_HEADER_LEN		    16			//min nes header valid byte len
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

enum Instruction {
    ADC, AND, ASL, BCC, BCS, BEQ, BIT, BMI, BNE, BPL, BRK, BVC, BVS, CLC,
    CLD, CLI, CLV, CMP, CPX, CPY, DEC, DEX, DEY, EOR, INC, INX, INY, JMP,
    JSR, LDA, LDX, LDY, LSR, NOP, ORA, PHA, PHP, PLA, PLP, ROL, ROR, RTI,
    RTS, SBC, SEC, SED, SEI, STA, STX, STY, TAX, TAY, TSX, TXA, TXS, TYA, UNDEFINED
};

const int8* instruction_names[] = {
    "ADC", "AND", "ASL", "BCC", "BCS", "BEQ", "BIT", "BMI", "BNE", "BPL", "BRK", "BVC", "BVS", "CLC",
    "CLD", "CLI", "CLV", "CMP", "CPX", "CPY", "DEC", "DEX", "DEY", "EOR", "INC", "INX", "INY", "JMP",
    "JSR", "LDA", "LDX", "LDY", "LSR", "NOP", "ORA", "PHA", "PHP", "PLA", "PLP", "ROL", "ROR", "RTI",
    "RTS", "SBC", "SEC", "SED", "SEI", "STA", "STX", "STY", "TAX", "TAY", "TSX", "TXA", "TXS", "TYA", "UNDEFINED"
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

    int8 instruct = UNDEFINED;
    const int8* instruct_name = "UNDEFINED";
    int8 add_mode = IMPLIED;
    int8 bytes;
    int8 cycles;
};

OpCode* op_list = new OpCode[256];

const int32 pre_op_list[] = {
    0, BRK, IMPLIED,            1, ORA, INDIRECT_X,             5, ORA, ZERO_PAGE,              6, ASL, ZERO_PAGE,
    8, PHP, IMPLIED,            9, ORA, IMMEDIATE,              10, ASL, ACCUMULATOR,           13, ORA, ABSOLUTE,
    14, ASL, ABSOLUTE,          16, BPL, IMPLIED,               17, ORA, INDIRECT_Y,            21, ORA, ZERO_PAGE_X, 
    22, ASL, ZERO_PAGE_X,       24, CLC, IMPLIED,               25, ORA, ABSOLUTE_Y,            29, ORA, ABSOLUTE_X, 
    30, ASL, ABSOLUTE_X,        32, JSR, IMPLIED,               33, AND, INDIRECT_X,            36, BIT, ZERO_PAGE, 
    37, AND, ZERO_PAGE,         38, ROL, ZERO_PAGE,             40, PLP, IMPLIED,               41, AND, IMMEDIATE, 
    42, ROL, ACCUMULATOR,       44, BIT, ABSOLUTE,              45, AND, ABSOLUTE,              46, ROL, ABSOLUTE, 
    48, BMI, IMPLIED,           49, AND, INDIRECT_Y,            53, AND, ZERO_PAGE_X,           54, ROL, ZERO_PAGE_X, 
    56, SEC, IMPLIED,           57, AND, ABSOLUTE_Y,            61, AND, ABSOLUTE_X,            62, ROL, ABSOLUTE_X, 
    64, RTI, IMPLIED,           65, EOR, INDIRECT_X,            69, EOR, ZERO_PAGE,             70, LSR, ZERO_PAGE, 
    72, PHA, IMPLIED,           73, EOR, IMMEDIATE,             74, LSR, ACCUMULATOR,           76, JMP, ABSOLUTE, 
    77, EOR, ABSOLUTE,          78, LSR, ABSOLUTE,              80, BVC, IMPLIED,               81, EOR, INDIRECT_Y, 
    85, EOR, ZERO_PAGE_X,       86, LSR, ZERO_PAGE_X,           88, CLI, IMPLIED,               89, EOR, ABSOLUTE_Y, 
    93, EOR, ABSOLUTE_X,        94, LSR, ABSOLUTE_X,            96, RTS, IMPLIED,               97, ADC, INDIRECT_X, 
    101, ADC, ZERO_PAGE,        102, ROR, ZERO_PAGE,            104, PLA, IMPLIED,              105, ADC, IMMEDIATE, 
    106, ROR, ACCUMULATOR,      108, JMP, INDIRECT,             109, ADC, ABSOLUTE,             110, ROR, ABSOLUTE, 
    112, BVS, IMPLIED,          113, ADC, INDIRECT_Y,           117, ADC, ZERO_PAGE_X,          118, ROR, ZERO_PAGE_X, 
    120, SEI, IMPLIED,          121, ADC, ABSOLUTE_Y,           125, ADC, ABSOLUTE_X,           126, ROR, ABSOLUTE_X, 
    129, STA, INDIRECT_X,       132, STY, ZERO_PAGE,            133, STA, ZERO_PAGE,            134, STX, ZERO_PAGE, 
    136, DEY, IMPLIED,          138, TXA, IMPLIED,              140, STY, ABSOLUTE,             141, STA, ABSOLUTE, 
    142, STX, ABSOLUTE,         144, BCC, IMPLIED,              145, STA, INDIRECT_Y,           148, STY, ZERO_PAGE_X, 
    149, STA, ZERO_PAGE_X,      150, STX, ZERO_PAGE_Y,          152, TYA, IMPLIED,              153, STA, ABSOLUTE_Y, 
    154, TXS, IMPLIED,          157, STA, ABSOLUTE_X,           160, LDY, IMMEDIATE,            161, LDA, INDIRECT_X, 
    162, LDX, IMMEDIATE,        164, LDY, ZERO_PAGE,            165, LDA, ZERO_PAGE,            166, LDX, ZERO_PAGE, 
    168, TAY, IMPLIED,          169, LDA, IMMEDIATE,            170, TAX, IMPLIED,              172, LDY, ABSOLUTE, 
    173, LDA, ABSOLUTE,         174, LDX, ABSOLUTE,             176, BCS, IMPLIED,              177, LDA, INDIRECT_Y, 
    180, LDY, ZERO_PAGE_X,      181, LDA, ZERO_PAGE_X,          182, LDX, ZERO_PAGE_Y,          184, CLV, IMPLIED, 
    185, LDA, ABSOLUTE_Y,       186, TSX, IMPLIED,              188, LDY, ABSOLUTE_X,           189, LDA, ABSOLUTE_X, 
    190, LDX, ABSOLUTE_Y,       192, CPY, IMMEDIATE,            193, CMP, INDIRECT_X,           196, CPY, ZERO_PAGE, 
    197, CMP, ZERO_PAGE,        198, DEC, ZERO_PAGE,            200, INY, IMPLIED,              201, CMP, IMMEDIATE, 
    202, DEX, IMPLIED,          204, CPY, ABSOLUTE,             205, CMP, ABSOLUTE,             206, DEC, ABSOLUTE, 
    208, BNE, IMPLIED,          209, CMP, INDIRECT_Y,           213, CMP, ZERO_PAGE_X,          214, DEC, ZERO_PAGE_X, 
    216, CLD, IMPLIED,          217, CMP, ABSOLUTE_Y,           221, CMP, ABSOLUTE_X,           222, DEC, ABSOLUTE_X, 
    224, CPX, IMMEDIATE,        225, SBC, INDIRECT_X,           228, CPX, ZERO_PAGE,            229, SBC, ZERO_PAGE, 
    230, INC, ZERO_PAGE,        232, INX, IMPLIED,              233, SBC, IMMEDIATE,            234, NOP, IMPLIED, 
    236, CPX, ABSOLUTE,         237, SBC, ABSOLUTE,             238, INC, ABSOLUTE,             240, BEQ, IMPLIED, 
    241, SBC, INDIRECT_Y,       245, SBC, ZERO_PAGE_X,          246, INC, ZERO_PAGE_X,          248, SED, IMPLIED, 
    249, SBC, ABSOLUTE_Y,       253, SBC, ABSOLUTE_X,           254, INC, ABSOLUTE_X
};

int main(int argc, char* argv[]) {
    for (int n = 0; n < 453; n += 3) {
        OpCode& op = op_list[pre_op_list[n]];
        op.instruct = (int8)pre_op_list[n + 1];
        op.add_mode = (int8)pre_op_list[n + 2];
        op.instruct_name = instruction_names[pre_op_list[n + 1]];
    }

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

            for (int n = 0; n < prg_data.length(); ++n) {
                uint8 op = prg_data[n];
                std::cout << op_list[op].instruct_name << "\n";
            }

            int a = 5;
		}
	}

	return 0;
}
