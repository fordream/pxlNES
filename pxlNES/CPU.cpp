#include "CPU.h"

int16 num_PRGs;
int16 num_CHRs;

uint8 a = 0;
uint8 x = 0;
uint8 y = 0;
uint16 pc = 0x8000;
uint8 sp = 0xfd;
uint8 p = 0;

uint8* RAM = new uint8[2048];

std::string prg_data;

void CPU_process() {
    OpCode& op = op_list[(uint8)prg_data[pc - 0x8000]];

    uint8 val = 0;
    ++pc;
    switch (op.add_mode) {
        case IMPLICIT:
            break;
        case ACCUMULATOR:
            val = a;
            break;
        case IMMEDIATE:
            val = (uint8)prg_data[pc - 0x8000];
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
        case LDA: a = val; break;
        case LDX: x = val; break;
        case LDY: y = val; break;
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

    std::cout << op.instruct_name << ", " << op.add_mode_name << ", val: " << (uint32)val << "\n";
    std::cout << "a: " << (uint32)a << ", x: " << (uint32)x << ", y: " << (uint32)y << 
        ", pc: " << (uint32)pc << ", sp: " << (uint32)sp << ", p: " << (uint32)p << "\n";
}

void CPU_run() {
    for (int n = 0; n < prg_data.length(); ++n) {
        CPU_process();
    }
}

void CPU_init() {
    for (int n = 0; n < 453; n += 3) {
        OpCode& op = op_list[pre_op_list[n]];
        op.instruct = (int8)pre_op_list[n + 1];
        op.add_mode = (int8)pre_op_list[n + 2];
        op.instruct_name = instruction_names[pre_op_list[n + 1]];
        op.add_mode_name = add_mode_names[pre_op_list[n + 2]];
    }
}