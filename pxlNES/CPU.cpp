#include "CPU.h"

int16 num_PRGs;
int16 num_CHRs;

uint8 a = 0;
uint8 x = 0;
uint8 y = 0;
uint16 pc = 0x8000;
uint8 sp = 0xfd;
uint8 p = 0x34;

std::string prg_data;

uint8 RAM[RAM_SIZE];
OpCode op_list[MAX_OPCODES];

std::thread CPU_thread;

void CPU_process() {
    OpCode& op = op_list[(uint8)prg_data[pc - 0x8000]];

    uint8 operand1 = 0;
    uint8 operand2 = 0;
    uint8* mem = NULL;
    uint16 pc_offset = 1;
    switch (op.add_mode) {
        case IMPLICIT:
            break;
        case ACCUMULATOR:
            mem = &a;
            break;
        case IMMEDIATE:
            operand1 = prg_data[(pc + 1) - 0x8000];
            mem = (uint8*)&prg_data[(pc + 1) - 0x8000];
            ++pc_offset;
            break;
        case ZERO_PAGE:
            ++pc_offset;
            break;
        case ZERO_PAGE_X:
            ++pc_offset;
            break;
        case ZERO_PAGE_Y:
            ++pc_offset;
            break;
        case RELATIVE:
            mem = (uint8*)&prg_data[(pc + 1) - 0x8000];
            ++pc_offset;
            break;
        case ABSOLUTE:
            operand1 = prg_data[(pc + 1) - 0x8000];
            operand2 = prg_data[(pc + 2) - 0x8000];
            mem = CPU_read(operand1, operand2);
            pc_offset += 2;
            break;
        case ABSOLUTE_X:
            pc_offset += 2;
            break;
        case ABSOLUTE_Y:
            pc_offset += 2;
            break;
        case INDIRECT:
            pc_offset += 2;
            break;
        case INDIRECT_X:
            pc_offset += 2;
            break;
        case INDIRECT_Y:
            pc_offset += 2;
            break;
    }

    switch (op.instruct) {
        case ADC: break;
        case AND: break;
        case ASL: break;
        case BCC: break;
        case BCS: break;
        case BEQ: if (p & 2 == 0) { pc += (int8)operand1; } break;
        case BIT: break;
        case BMI: break;
        case BNE: break;
        case BPL: if ((p & 128) == 0) { pc += (int8)operand1; } break;
        case BRK: break;
        case BVC: break;
        case BVS: break;
        case CLC: p = p & ~1;  break;   //clear carry flag        (11111110)
        case CLD: p = p & ~8;  break;   //clear decimal mode flag (11110111)
        case CLI: p = p & ~4;  break;   //clear interrupt flag    (11111011)
        case CLV: p = p & ~64; break;   //clear overflow flag     (10111111)
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
        case LDA: a = *mem; break;  //put operand1 in a
        case LDX: x = *mem; break;  //put operand1 in x
        case LDY: y = *mem; break;  //put operand1 in y
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
        case STA: CPU_store(operand2, operand1, a); break;
        case STX: CPU_store(operand2, operand1, x); break;
        case STY: CPU_store(operand2, operand1, y); break;
        case TAX: break;
        case TAY: break;
        case TSX: break;
        case TXA: break;
        case TXS: break;
        case TYA: break;
        default:  break;
    }

    pc += pc_offset;

    std::cout << op.instruct_name << ", " << op.add_mode_name << ", val: " << (uint32)operand1 << "\n";
    std::cout << "a: " << (uint32)a << ", x: " << (uint32)x << ", y: " << (uint32)y <<
        ", pc: " << (uint32)pc << ", sp: " << (uint32)sp << ", p: " << (uint32)p << "\n";
    std::cout << "ppuctrl: " << (uint32)PPU_registers[0] << ", mask: " << (uint32)PPU_registers[1] << 
        ", status: " << (uint32)PPU_registers[2] << ", oamaddr: " << (uint32)PPU_registers[3] << 
        ", oamdata: " << (uint32)PPU_registers[4] << ", scroll: " << (uint32)PPU_registers[5] <<
        ", addr: " << (uint32)PPU_registers[6] << ", data: " << (uint32)PPU_registers[7] << "\n";
}

uint8* CPU_read(uint8 addr_hi, uint8 addr_lo) {
    if (addr_hi <= 0x1f) {                          //RAM + mirrors from $0000 - $07ff
        return &RAM[addr_lo % 0x800];
    }else if (addr_hi >= 0x20 && addr_hi <= 0x3f) { //PPU registers + mirrors from $2000 - $3ffff
        return (uint8*)PPU_registers[addr_lo % 8];
    }
    return NULL;
}

void CPU_store(uint8 addr_hi, uint8 addr_lo, uint8 val) {
    if (addr_hi <= 0x1f) {                          //RAM + mirrors from $0000 - $07ff
        RAM[((addr_hi * 256) + addr_lo) % 0x800];
    }else if (addr_hi >= 0x20 && addr_hi <= 0x3f) { //PPU registers + mirrors from $2000 - $3ffff
        PPU_registers[addr_lo % 8] = val;
    }
}

void CPU_run() {
    for (int n = 0; n < prg_data.length(); ++n) {
        CPU_process();
    }
}

void CPU_init() {
    //loads the pre op data list into the final op list
    for (int n = 0; n < PREOP_SIZE; n += 3) {
        OpCode& op = op_list[pre_op_list[n]];
        op.instruct = (uint8)pre_op_list[n + 1];
        op.add_mode = (uint8)pre_op_list[n + 2];
        op.instruct_name = instruction_names[pre_op_list[n + 1]];
        op.add_mode_name = add_mode_names[pre_op_list[n + 2]];
    }

    //set all RAM values to 0xFF
    for (int n = 0; n < RAM_SIZE; ++n) {
        RAM[n] = 0xff;
    }
    //extra bytes outlined on nesdev wiki just in case
    RAM[8] = 0xf7;
    RAM[9] = 0x3f;
    RAM[10] = 0xdf;
    RAM[15] = 0xbf;
}

void CPU_dispose() {
    delete[] op_list;
    delete[] pre_op_list;
    delete[] instruction_names;
    delete[] add_mode_names;
    delete[] RAM;
}