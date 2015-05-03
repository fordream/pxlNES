#include <iostream>
#include <fstream>
#include <stdint.h>

//iNES header constants
#define INES_HEADER_LEN         16			//min nes header valid byte len
#define SUB_CHAR                26			//used as escape sequence in MS-DOS

typedef char                    int8;
typedef short                   int16;
typedef int                     int32;
typedef long long               int64;
typedef unsigned char           uint8;
typedef unsigned short          uint16;
typedef unsigned int            uint32;
typedef unsigned long long      uint64;

#define PRG_LEN 16384   //16kb
#define CHR_LEN 8192    //8kb

int16 num_PRGs;
int16 num_CHRs;

uint8 a = 0;
uint8 x = 0;
uint8 y = 0;
uint16 pc = 0x8000;
uint8 sp = 0xfd;
uint8 p = 0x24;

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

const int8* add_mode_names[] = {
    "IMPLICIT", "ACCUMULATOR", "IMMEDIATE", "ZERO_PAGE", "ZERO_PAGE_X", "ZERO_PAGE_Y", "RELATIVE",
    "ABSOLUTE", "ABSOLUTE_X", "ABSOLUTE_Y", "INDIRECT", "INDIRECT_X", "INDIRECT_Y"
};

struct OpCode {

    int8 instruct = UNDEFINED;
    const int8* instruct_name = "UNDEFINED";
    int8 add_mode = IMPLICIT;
    const int8* add_mode_name = "IMPLICIT";
    int8 bytes;
    int8 cycles;
};

OpCode* op_list = new OpCode[256];

const int32 pre_op_list[] = {
    0, BRK, IMPLICIT,           1, ORA, INDIRECT_X,             5, ORA, ZERO_PAGE,              6, ASL, ZERO_PAGE,
    8, PHP, IMPLICIT,           9, ORA, IMMEDIATE,              10, ASL, ACCUMULATOR,           13, ORA, ABSOLUTE,
    14, ASL, ABSOLUTE,          16, BPL, RELATIVE,              17, ORA, INDIRECT_Y,            21, ORA, ZERO_PAGE_X, 
    22, ASL, ZERO_PAGE_X,       24, CLC, IMPLICIT,              25, ORA, ABSOLUTE_Y,            29, ORA, ABSOLUTE_X, 
    30, ASL, ABSOLUTE_X,        32, JSR, IMPLICIT,              33, AND, INDIRECT_X,            36, BIT, ZERO_PAGE, 
    37, AND, ZERO_PAGE,         38, ROL, ZERO_PAGE,             40, PLP, IMPLICIT,              41, AND, IMMEDIATE, 
    42, ROL, ACCUMULATOR,       44, BIT, ABSOLUTE,              45, AND, ABSOLUTE,              46, ROL, ABSOLUTE, 
    48, BMI, RELATIVE,          49, AND, INDIRECT_Y,            53, AND, ZERO_PAGE_X,           54, ROL, ZERO_PAGE_X, 
    56, SEC, IMPLICIT,          57, AND, ABSOLUTE_Y,            61, AND, ABSOLUTE_X,            62, ROL, ABSOLUTE_X, 
    64, RTI, IMPLICIT,          65, EOR, INDIRECT_X,            69, EOR, ZERO_PAGE,             70, LSR, ZERO_PAGE, 
    72, PHA, IMPLICIT,          73, EOR, IMMEDIATE,             74, LSR, ACCUMULATOR,           76, JMP, ABSOLUTE, 
    77, EOR, ABSOLUTE,          78, LSR, ABSOLUTE,              80, BVC, RELATIVE,              81, EOR, INDIRECT_Y, 
    85, EOR, ZERO_PAGE_X,       86, LSR, ZERO_PAGE_X,           88, CLI, IMPLICIT,              89, EOR, ABSOLUTE_Y, 
    93, EOR, ABSOLUTE_X,        94, LSR, ABSOLUTE_X,            96, RTS, IMPLICIT,              97, ADC, INDIRECT_X, 
    101, ADC, ZERO_PAGE,        102, ROR, ZERO_PAGE,            104, PLA, IMPLICIT,             105, ADC, IMMEDIATE, 
    106, ROR, ACCUMULATOR,      108, JMP, INDIRECT,             109, ADC, ABSOLUTE,             110, ROR, ABSOLUTE, 
    112, BVS, RELATIVE,         113, ADC, INDIRECT_Y,           117, ADC, ZERO_PAGE_X,          118, ROR, ZERO_PAGE_X, 
    120, SEI, IMPLICIT,         121, ADC, ABSOLUTE_Y,           125, ADC, ABSOLUTE_X,           126, ROR, ABSOLUTE_X, 
    129, STA, INDIRECT_X,       132, STY, ZERO_PAGE,            133, STA, ZERO_PAGE,            134, STX, ZERO_PAGE, 
    136, DEY, IMPLICIT,         138, TXA, IMPLICIT,             140, STY, ABSOLUTE,             141, STA, ABSOLUTE, 
    142, STX, ABSOLUTE,         144, BCC, RELATIVE,             145, STA, INDIRECT_Y,           148, STY, ZERO_PAGE_X, 
    149, STA, ZERO_PAGE_X,      150, STX, ZERO_PAGE_Y,          152, TYA, IMPLICIT,             153, STA, ABSOLUTE_Y, 
    154, TXS, IMPLICIT,         157, STA, ABSOLUTE_X,           160, LDY, IMMEDIATE,            161, LDA, INDIRECT_X, 
    162, LDX, IMMEDIATE,        164, LDY, ZERO_PAGE,            165, LDA, ZERO_PAGE,            166, LDX, ZERO_PAGE, 
    168, TAY, IMPLICIT,         169, LDA, IMMEDIATE,            170, TAX, IMPLICIT,             172, LDY, ABSOLUTE, 
    173, LDA, ABSOLUTE,         174, LDX, ABSOLUTE,             176, BCS, RELATIVE,             177, LDA, INDIRECT_Y, 
    180, LDY, ZERO_PAGE_X,      181, LDA, ZERO_PAGE_X,          182, LDX, ZERO_PAGE_Y,          184, CLV, IMPLICIT, 
    185, LDA, ABSOLUTE_Y,       186, TSX, IMPLICIT,             188, LDY, ABSOLUTE_X,           189, LDA, ABSOLUTE_X, 
    190, LDX, ABSOLUTE_Y,       192, CPY, IMMEDIATE,            193, CMP, INDIRECT_X,           196, CPY, ZERO_PAGE, 
    197, CMP, ZERO_PAGE,        198, DEC, ZERO_PAGE,            200, INY, IMPLICIT,             201, CMP, IMMEDIATE, 
    202, DEX, IMPLICIT,         204, CPY, ABSOLUTE,             205, CMP, ABSOLUTE,             206, DEC, ABSOLUTE, 
    208, BNE, RELATIVE,         209, CMP, INDIRECT_Y,           213, CMP, ZERO_PAGE_X,          214, DEC, ZERO_PAGE_X, 
    216, CLD, IMPLICIT,         217, CMP, ABSOLUTE_Y,           221, CMP, ABSOLUTE_X,           222, DEC, ABSOLUTE_X, 
    224, CPX, IMMEDIATE,        225, SBC, INDIRECT_X,           228, CPX, ZERO_PAGE,            229, SBC, ZERO_PAGE, 
    230, INC, ZERO_PAGE,        232, INX, IMPLICIT,             233, SBC, IMMEDIATE,            234, NOP, IMPLICIT, 
    236, CPX, ABSOLUTE,         237, SBC, ABSOLUTE,             238, INC, ABSOLUTE,             240, BEQ, RELATIVE, 
    241, SBC, INDIRECT_Y,       245, SBC, ZERO_PAGE_X,          246, INC, ZERO_PAGE_X,          248, SED, IMPLICIT, 
    249, SBC, ABSOLUTE_Y,       253, SBC, ABSOLUTE_X,           254, INC, ABSOLUTE_X
};

std::string prg_data;

void process_instruct() {
    OpCode& op = op_list[(uint8)prg_data[pc - 0x8000]];

    std::cout << op.instruct_name << ", " << op.add_mode_name << "\n";

    ++pc;
    switch (op.add_mode) {
        case IMPLICIT:
            break;
        case ACCUMULATOR:
            break;
        case IMMEDIATE:
            ++pc;
            break;
        case ZERO_PAGE:
            ++pc;
            break;
        case ZERO_PAGE_X:
            ++pc;
            break;
        case ZERO_PAGE_Y:
            ++pc;
            break;
        case RELATIVE:
            ++pc;
            break;
        case ABSOLUTE:
            pc += 2;
            break;
        case ABSOLUTE_X:
            pc += 2;
            break;
        case ABSOLUTE_Y:
            pc += 2;
            break;
        case INDIRECT:
            pc += 2;
            break;
        case INDIRECT_X:
            pc += 2;
            break;
        case INDIRECT_Y:
            pc += 2;
            break;
    }

    switch (op.instruct) {
        case ADC: break;
        case AND: break; 
        case ASL: break;
        case BCC: break;
        case BCS: break;
        case BEQ: break;
        case BIT: break;
        case BMI: break;
        case BNE: break;
        case BPL: break;
        case BRK: break;
        case BVC: break;
        case BVS: break;
        case CLC: p = p & 254; break; //clear carry flag        (11111110)
        case CLD: p = p & 247; break; //clear decimal mode flag (11110111)
        case CLI: p = p & 251; break; //clear interrupt flag    (11111011)
        case CLV: p = p & 191; break; //clear overflow flag     (10111111)
        case CMP: break;
        case CPX: break;
        case CPY: break;
        case DEC: break;
        case DEX: break;
        case DEY: break;
        case EOR: break;
        case INC: break;
        case INX: break;
        case INY: break;
        case JMP: break;
        case JSR: break;
        case LDA: break;
        case LDX: break;
        case LDY: break;
        case LSR: break;
        case NOP: break;
        case ORA: break;
        case PHA: break;
        case PHP: break;
        case PLA: break;
        case PLP: break;
        case ROL: break;
        case ROR: break;
        case RTI: break;
        case RTS: break;
        case SBC: break;
        case SEC: break;
        case SED: break;
        case SEI: p = p | 4; break;     //set interrupt flag (00000100)
        case STA: break;
        case STX: break;
        case STY: break;
        case TAX: break;
        case TAY: break;
        case TSX: break;
        case TXA: break;
        case TXS: break;
        case TYA: break;
        default:  break;
    }
}

int main(int argc, char* argv[]) {
    for (int n = 0; n < 453; n += 3) {
        OpCode& op = op_list[pre_op_list[n]];
        op.instruct = (int8)pre_op_list[n + 1];
        op.add_mode = (int8)pre_op_list[n + 2];
        op.instruct_name = instruction_names[pre_op_list[n + 1]];
        op.add_mode_name = add_mode_names[pre_op_list[n + 2]];
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

            prg_data = str.substr(INES_HEADER_LEN, str.length());

            for (int n = 0; n < prg_data.length(); ++n) {
                process_instruct();
            }
        }
    }

    return 0;
}
