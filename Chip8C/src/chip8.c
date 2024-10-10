#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#include <chip8.h>
#include <instructions.h>

int decode_ins(unsigned short ins);

struct timespec ts = {
    .tv_sec = DELAY_US / 1000000,
    .tv_nsec = (DELAY_US % 1000000) * 1000ULL
};

int execute(unsigned short ins, struct chip8 *c8)
{
    unsigned char x = (ins & 0xF00) >> 8;
    unsigned char y = (ins & 0xF0) >> 4;
    unsigned char kk = ins & 0xFF;
    unsigned short nnn = ins & 0xFFF;
    
    switch(decode_ins(ins))
    {
        case SYS:
            // Jump to a machine code routine at nnn
            // do nothing pls
            break;
        case CLS:
            // Clear the display
            memset(c8->display_buf, 0, VIDEO_HEIGHT*VIDEO_WIDTH);
            return DRAW_FLAG;
        case RET:
            // Return from a subroutine
            c8->PC = c8->stack[c8->SP--];
            break;
        case JPn:
            // Jump to location nnn
            c8->PC = nnn;
            break;
        case CALL:
            // Call subroutine at nnn
            c8->stack[++c8->SP] = c8->PC;
            c8->PC = nnn;
            break;
        case SExb:
            // Skip next instruction if Vx == kk
            if (c8->V[x] == kk) c8->PC += 2;
            break;
        case SNExb:
            // Skip next instruction if Vx != kk
            if (c8->V[x] != kk) c8->PC += 2;
            break;
        case SExy:
            // Skip next instruction if Vx == Vy
            if (c8->V[x] == c8->V[y]) c8->PC += 2;
            break;
        case LDxb:
            // Set Vx = kk
            c8->V[x] = kk;
            break;
        case ADDxb:
            // Set Vx = Vx + kk
            c8->V[x] += kk;
            break;
        case LDxy:
            // Set Vx = Vy
            c8->V[x] = c8->V[y];
            break;
        case OR:
            // Set Vx = Vx OR Vy
            c8->V[x] |= c8->V[y];
            break;
        case AND:
            // Set Vx = Vx AND Vy
            c8->V[x] &= c8->V[y];
            break;
        case XOR:
            // Set Vx = Vx XOR Vy
            c8->V[x] ^= c8->V[y];
            break;
        case ADDxy:
            // Set Vx = Vx + Vy, set VF = carry
            c8->V[0xF] = ((int)c8->V[x] + (int)c8->V[y]) > 255 ? 1 : 0;
            c8->V[x] += c8->V[y];
            break;
        case SUBxy:
            // Set Vx = Vx - Vy, set VF = NOT borrow
            c8->V[0xF] = c8->V[x] > c8->V[y] ? 1 : 0;
            c8->V[x] -= c8->V[y];
            break;
        case SHR:
            // Set Vx = Vx SHR 1
            c8->V[0xF] = c8->V[x] & 1;
            c8->V[x] >>= 1;
            break;
        case SUBN:
            // Set Vx = Vy - Vx, set VF = NOT borrow
            c8->V[0xF] = c8->V[y] > c8->V[x] ? 1 : 0;
            c8->V[x] = c8->V[y] - c8->V[x];
            break;
        case SHL:
            // Set Vx = Vx SHL 1
            c8->V[0xF] = (c8->V[x] >> 7) & 0x1;
            c8->V[x] <<= 1;
            break;
        case SNExy:
            // Skip next instruction if Vx != Vy
            if (c8->V[x] != c8->V[y]) c8->PC += 2;
            break;
        case LDI:
            // Set I = nnn
            c8->I = nnn;
            break;
        case JPVn:
            // Jump to location nnn + V0
            c8->PC = nnn + c8->V[0];
            break;
        case RND:
            // Set Vx = random byte AND kk
            c8->V[x] = ((unsigned char) rand()) & kk;
            break;
        case DRW:
            // Display n-byte c8->SPrite starting at memory location I at (Vx, Vy), set VF = collision
            unsigned char height = ins & 0xF;
            int x_pos = c8->V[x] % VIDEO_WIDTH;
            int y_pos = c8->V[y] % VIDEO_HEIGHT;

            c8->V[0xF] = 0;

            for (int row = 0; row < height; row++)
            {
                unsigned char sprite_row = c8->memory[c8->I+row];
                for (int col = 0; col < 8; col++)
                {
                    unsigned char sprite_pixel = sprite_row & (0x80 >> col);

                    // make sure to mod it so it wraps around
                    int buffer_pos = ((y_pos + row) * VIDEO_WIDTH + (x_pos + col)) % (VIDEO_HEIGHT*VIDEO_WIDTH);

                    int screen_pixel = c8->display_buf[buffer_pos];

                    if(sprite_pixel > 0)
                    {
                        if (screen_pixel == 1)
                        {
                            c8->V[0xF] = 1;
                        }
                        c8->display_buf[buffer_pos] ^= 1;
                    }
                }
            }
            return DRAW_FLAG;
        case SKNP:
            // Skip next instruction if key with the value of Vx is not pressed
            if (!c8->keys[c8->V[x]]) c8->PC += 2;
            break;
        case SKP:
            // Skip next instruction if key with the value of Vx is pressed
            if (c8->keys[c8->V[x]]) c8->PC += 2;
            break;
        case LDxDT:
            // Set Vx = delay timer value
            c8->V[x] = c8->DT;
            break;
        case LDxK:
            // Wait for a key press, store the value of the key in Vx
            bool key_press = false;
            for (unsigned char i = 0; i < 16; i++)
            {
                if (c8->keys[i])
                {
                    c8->V[x] = i;
                    key_press = true;
                }
            }
            if (!key_press)
            {
                c8->PC -= 2;
            }
            break;
        case LDDTx:
            // Set delay timer = Vx
            c8->DT = c8->V[x];
            break;
        case LDST:
            // Set sound timer = Vx
            c8->ST = c8->V[x];
            break;
        case ADDI:
            // Set I = I + Vx
            c8->I += c8->V[x];
            break;
        case LDF:
            // Set I = location of c8->SPrite for digit Vx
            c8->I = c8->V[x]*5;
            break;
        case LDB:
            // Store BCD representation of Vx in memory locations I, I+1, and I+2
            c8->memory[c8->I] = (c8->V[x]/100)%10;
            c8->memory[c8->I+1] = (c8->V[x]/10)%10;
            c8->memory[c8->I+2] = c8->V[x]%10;
            break;
        case LDIx:
            // Store registers V0 through Vx in memory starting at location I
            memcpy(c8->memory + c8->I, c8->V, x+1);
            break;
        case LDxI:
            // Read registers V0 through Vx from memory starting at location I
            memcpy(c8->V, c8->memory + c8->I, x+1);
            break;
        default:
            // Handle unknown instruction
            printf("Unknown instruction 0x%x\n", ins);
            return -1;
    }
    return 0;
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
            switch (ins & 0xFF)
            {
                case 0x9E:
                    return SKP;
                case 0xA1:
                    return SKNP;
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
                    return LDIx;
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

unsigned short fetch(struct chip8 *c8)
{
    unsigned short ins = 0;
    ins |= c8->memory[c8->PC] << 8;
    ins |= c8->memory[c8->PC+1];
    return ins;
}

int cycle(struct chip8 *c8)
{
    // one FDE loop
    int draw_flag = 0;

    unsigned short ins = fetch(c8);
    c8->PC += 2;
    
    draw_flag = execute(ins, c8);

    if (draw_flag == -1)
    {
        printf("Error occurred.\n");
    }

    if (c8->PC == MEM_SIZE-2)
    {
        printf("End of memory reached.\n");
        draw_flag = -1;
    }

    //decrement sound and delay timers

    if (c8->DT > 0) c8->DT--;
    if (c8->ST > 0) {c8->ST--; }
    return draw_flag;
}

void init(struct chip8 *c8)
{
    c8->PC = 512;

    //load hex digit c8->SPrites into interpreter c8->SPace

    unsigned char hex_codes[80] = 
    {
        0xF0,  0x90,  0x90,  0x90,  0xF0, // 0
        0x20,  0x60,  0x20,  0x20,  0x70, // 1
        0xF0,  0x10,  0xF0,  0x80,  0xF0, // 2
        0xF0,  0x10,  0xF0,  0x10,  0xF0, // 3
        0x90,  0x90,  0xF0,  0x10,  0x10, // 4
        0xF0,  0x80,  0xF0,  0x10,  0xF0, // 5
        0xF0,  0x80,  0xF0,  0x90,  0xF0, // 6
        0xF0,  0x10,  0x20,  0x40,  0x40, // 7
        0xF0,  0x90,  0xF0,  0x90,  0xF0, // 8
        0xF0,  0x90,  0xF0,  0x10,  0xF0, // 9
        0xF0,  0x90,  0xF0,  0x90,  0x90, // A
        0xE0,  0x90,  0xE0,  0x90,  0xE0, // B
        0xF0,  0x80,  0x80,  0x80,  0xF0, // C
        0xE0,  0x90,  0x90,  0x90,  0xE0, // D
        0xF0,  0x80,  0xF0,  0x80,  0xF0, // E
        0xF0,  0x80,  0xF0,  0x80,  0x80  // F
    };

    memcpy(c8->memory, hex_codes, 80);
}