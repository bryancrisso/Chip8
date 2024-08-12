#include "instructions.h"
#include "chip8.h"

int load_rom(char *filename, unsigned char *memory_buf, int start_loc);
int decode_ins(unsigned short ins);

int execute(unsigned short ins)
{
    unsigned char x = (ins & 0xF00) >> 8;
    unsigned char y = (ins & 0xF0) >> 4;
    unsigned char kk = ins & 0xFF;
    unsigned short nnn = ins & 0xFFF;
    
    switch(decode_ins(ins))
    {
        case CLS:
            // Clear the display
            break;
        case RET:
            // Return from a subroutine
            break;
        case SYS:
            // Jump to a machine code routine at nnn
            break;
        case JPn:
            // Jump to location nnn
            break;
        case CALL:
            // Call subroutine at nnn
            break;
        case SExb:
            // Skip next instruction if Vx == kk
            break;
        case SNExb:
            // Skip next instruction if Vx != kk
            break;
        case SExy:
            // Skip next instruction if Vx == Vy
            break;
        case LDxb:
            // Set Vx = kk
            break;
        case ADDxb:
            // Set Vx = Vx + kk
            break;
        case LDxy:
            // Set Vx = Vy
            break;
        case OR:
            // Set Vx = Vx OR Vy
            break;
        case AND:
            // Set Vx = Vx AND Vy
            break;
        case XOR:
            // Set Vx = Vx XOR Vy
            break;
        case ADDxy:
            // Set Vx = Vx + Vy, set VF = carry
            break;
        case SUBxy:
            // Set Vx = Vx - Vy, set VF = NOT borrow
            break;
        case SHR:
            // Set Vx = Vx SHR 1
            break;
        case SUBN:
            // Set Vx = Vy - Vx, set VF = NOT borrow
            break;
        case SHL:
            // Set Vx = Vx SHL 1
            break;
        case SNExy:
            // Skip next instruction if Vx != Vy
            break;
        case LDI:
            // Set I = nnn
            break;
        case JPVn:
            // Jump to location nnn + V0
            break;
        case RND:
            // Set Vx = random byte AND kk
            break;
        case DRW:
            // Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision
            break;
        case SKNP:
            // Skip next instruction if key with the value of Vx is not pressed
            break;
        case SKP:
            // Skip next instruction if key with the value of Vx is pressed
            break;
        case LDxDT:
            // Set Vx = delay timer value
            break;
        case LDxK:
            // Wait for a key press, store the value of the key in Vx
            break;
        case LDDTx:
            // Set delay timer = Vx
            break;
        case LDST:
            // Set sound timer = Vx
            break;
        case ADDI:
            // Set I = I + Vx
            break;
        case LDF:
            // Set I = location of sprite for digit Vx
            break;
        case LDB:
            // Store BCD representation of Vx in memory locations I, I+1, and I+2
            break;
        case LDIx:
            // Store registers V0 through Vx in memory starting at location I
            break;
        case LDxI:
            // Read registers V0 through Vx from memory starting at location I
            break;
        default:
            // Handle unknown instruction
            return -1;
        }
}

/**
 * Take a unsigned short instruction and decode it to an integer macro code
 */
int decode_ins(unsigned short ins)
{
    unsigned char first = (ins & 0xF000) >> 12;
    
    switch(first)
    {
        case 0:
            switch (ins & 0xFFF)
            {
                case 0x0E0:
                    return CLS;
                case 0x0EE:
                    return RET;
                default:
                    return SYS;
            }

        case 1:
            return JPn;
        case 2:
            return CALL;
        case 3:
            return SExb;
        case 4:
            return SNExb;
        case 5:
            return SExy;
        case 6:
            return LDxb;
        case 7:
            return ADDxb;

        case 8:
            switch(ins & 0xF)
            {
                case 0:
                    return LDxy;
                case 1:
                    return OR;
                case 2:
                    return AND;
                case 3:
                    return XOR;
                case 4:
                    return ADDxy;
                case 5:
                    return SUBxy;
                case 6:
                    return SHR;
                case 7:
                    return SUBN;
                case 0xE:
                    return SHL;
                default:
                    return -1;
            }

        case 9:
            return SNExy;
        case 0xA:
            return LDI;
        case 0xB:
            return JPVn;
        case 0xC:
            return RND;
        case 0xD:
            return DRW;
        
        case 0xE:
            switch (ins & 0xF)
            {
                case 1:
                    return SKNP;
                case 2:
                    return SKP;
                default:
                    return -1;
            }

        case 0xF:
            switch (ins & 0xFF)
            {
                case 0x7:
                    return LDxDT;
                case 0x0A:
                    return LDxK;
                case 0x15:
                    return LDDTx;
                case 0x18:
                    return LDST;
                case 0x1E:
                    return ADDI;
                case 0x29:
                    return LDF;
                case 0x33:
                    return LDB;
                case 0x55:
                    return LDI;
                case 0x65:
                    return LDxI;
                default:
                    return -1;
            }
        default:
            return -1;
    }
    return -1;
}