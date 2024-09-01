#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#include <instructions.h>
#include <chip8.h>

int decode_ins(unsigned short ins);

unsigned char memory[MEM_SIZE] = {0};
unsigned char V[16] = {0};
unsigned short I;
unsigned short PC;
unsigned char SP = 0;
unsigned short stack[16] = {0};

unsigned char DT;
unsigned char ST;
    
// last time since we clock cycled
float last_decrement = 0;
// CPU clock speed in Hz
float speed;

bool display_buf[VIDEO_WIDTH * VIDEO_HEIGHT] = {0};

bool keys[16] = {0};

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
            memset(memory, 0, MEM_SIZE);
            break;
        case RET:
            // Return from a subroutine
            PC = stack[SP--];
            break;
        case SYS:
            // Jump to a machine code routine at nnn
            // do nothing pls
            break;
        case JPn:
            // Jump to location nnn
            PC = nnn;
            break;
        case CALL:
            // Call subroutine at nnn
            stack[++SP] = PC;
            PC = nnn;
            break;
        case SExb:
            // Skip next instruction if Vx == kk
            if (V[x] == kk) PC += 2;
            break;
        case SNExb:
            // Skip next instruction if Vx != kk
            if (V[x] != kk) PC += 2;
            break;
        case SExy:
            // Skip next instruction if Vx == Vy
            if (V[x] == V[y]) PC += 2;
            break;
        case LDxb:
            // Set Vx = kk
            V[x] = kk;
            break;
        case ADDxb:
            // Set Vx = Vx + kk
            V[x] += kk;
            break;
        case LDxy:
            // Set Vx = Vy
            V[x] = V[y];
            break;
        case OR:
            // Set Vx = Vx OR Vy
            V[x] |= V[y];
            break;
        case AND:
            // Set Vx = Vx AND Vy
            V[x] &= V[y];
            break;
        case XOR:
            // Set Vx = Vx XOR Vy
            V[x] ^= V[y];
            break;
        case ADDxy:
            // Set Vx = Vx + Vy, set VF = carry
            V[x] += V[y];
            V[0xF] = ((int)V[x] + (int)V[y]) > 255 ? 1 : 0;
            break;
        case SUBxy:
            // Set Vx = Vx - Vy, set VF = NOT borrow
            V[0xF] = V[x] > V[y] ? 1 : 0;
            V[x] -= V[y];
            break;
        case SHR:
            // Set Vx = Vx SHR 1
            V[0xF] = V[x] & 1;
            V[x] >>= 1;
            break;
        case SUBN:
            // Set Vx = Vy - Vx, set VF = NOT borrow
            V[0xF] = V[y] > V[x] ? 1 : 0;
            V[x] = V[y] - V[x];
            break;
        case SHL:
            // Set Vx = Vx SHL 1
            V[0xF] = V[x] >> 7;
            V[x] <<= 1;
            break;
        case SNExy:
            // Skip next instruction if Vx != Vy
            if (V[x] != V[y]) PC += 2;
            break;
        case LDI:
            // Set I = nnn
            I = nnn;
            break;
        case JPVn:
            // Jump to location nnn + V0
            PC = nnn + V[0];
            break;
        case RND:
            // Set Vx = random byte AND kk
            V[x] = ((unsigned char) rand()) & kk;
            break;
        case DRW:
            // Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision
            unsigned char height = ins & 0xF;
            int x_pos = V[x] % VIDEO_WIDTH;
            int y_pos = V[y] % VIDEO_HEIGHT;

            V[0xF] = 0;

            for (int row = 0; row < height; row++)
            {
                unsigned char sprite_row = memory[I+row];
                for (int col = 0; col < 8; col++)
                {
                    unsigned char sprite_pixel = sprite_row & (0x80 >> col);

                    // make sure to mod it so it wraps around
                    int buffer_pos = ((y_pos + row) * VIDEO_WIDTH + (x_pos + col)) % (VIDEO_HEIGHT*VIDEO_WIDTH);

                    int screen_pixel = display_buf[buffer_pos];

                    if(sprite_pixel > 0)
                    {
                        if (screen_pixel == 1)
                        {
                            V[0xF] = 1;
                        }
                        display_buf[buffer_pos] ^= 1;
                    }
                }
            }

            break;
        case SKNP:
            // Skip next instruction if key with the value of Vx is not pressed
            if (keys[V[x]]) PC += 2;
            break;
        case SKP:
            // Skip next instruction if key with the value of Vx is pressed
            if (!keys[V[x]]) PC += 2;
            break;
        case LDxDT:
            // Set Vx = delay timer value
            V[x] = DT;
            break;
        case LDxK:
            // Wait for a key press, store the value of the key in Vx
            bool key_press = false;
            for (unsigned char i = 0; i < 16; i++)
            {
                if (keys[i])
                {
                    V[x] = i;
                    key_press = true;
                }
            }
            if (!key_press)
            {
                PC -= 2;
            }
            break;
        case LDDTx:
            // Set delay timer = Vx
            DT = V[x];
            break;
        case LDST:
            // Set sound timer = Vx
            ST = V[x];
            break;
        case ADDI:
            // Set I = I + Vx
            I += V[x];
            break;
        case LDF:
            // Set I = location of sprite for digit Vx
            I = V[x]*5;
            break;
        case LDB:
            // Store BCD representation of Vx in memory locations I, I+1, and I+2
            memory[I] = (V[x]/100)%10;
            memory[I+1] = (V[x]/10)%10;
            memory[I+2] = V[x]%10;
            break;
        case LDIx:
            // Store registers V0 through Vx in memory starting at location I
            memcpy(memory + I, V, x+1);
            break;
        case LDxI:
            // Read registers V0 through Vx from memory starting at location I
            memcpy(V, memory + I, x+1);
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

unsigned short fetch()
{
    unsigned short ins = 0;
    ins |= memory[PC] << 8;
    ins |= memory[PC+1];
    return ins;
}

void cycle()
{
    // one FDE loop

    unsigned short ins = fetch();
    PC += 2;
    
    if (execute(ins) == -1)
    {
        printf("Error occurred.\n");
        return;
    }

    //decrement sound and delay timers

    if (DT > 0) DT--;
    if (ST > 0) {ST--; printf("Beep.\n");}
}

void init()
{
    PC = 512;

    //load hex digit sprites into interpreter space

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

    memcpy(memory, hex_codes, 80);

    // start an output window

    // start input handler

    // FDE Cycle

    while (PC < MEM_SIZE-1)
    {
        cycle();
        nanosleep(&ts, &ts);
    }

    printf("END\n");
}