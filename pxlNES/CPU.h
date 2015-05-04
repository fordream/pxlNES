#ifndef _CPU_H_
#define _CPU_H_

#include <iostream>
#include "Constants.h"
#include "PPU.h"

enum Instruction {
    ADC, AND, ASL, BCC, BCS, BEQ, BIT, BMI, BNE, BPL, BRK, BVC, BVS, CLC,
    CLD, CLI, CLV, CMP, CPX, CPY, DEC, DEX, DEY, EOR, INC, INX, INY, JMP,
    JSR, LDA, LDX, LDY, LSR, NOP, ORA, PHA, PHP, PLA, PLP, ROL, ROR, RTI,
    RTS, SBC, SEC, SED, SEI, STA, STX, STY, TAX, TAY, TSX, TXA, TXS, TYA, UNDEFINED
};

static const int8* instruction_names[] = {
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

static const int8* add_mode_names[] = {
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

extern OpCode op_list[MAX_OPCODES];

static const int32 pre_op_list[] = {
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

extern int16 num_PRGs;
extern int16 num_CHRs;

extern uint8 a;
extern uint8 x;
extern uint8 y;
extern uint16 pc;
extern uint8 sp;
extern uint8 p;

extern uint8 RAM[RAM_SIZE];

extern std::string prg_data;

extern void CPU_init();
extern void CPU_run();
extern void CPU_process();

#endif