#ifndef CHIP8_H
#define CHIP8_H

#include <time.h>

#define MEM_SIZE 4096
#define VIDEO_WIDTH 64
#define VIDEO_HEIGHT 32
#define CLOCK_HZ 500
#define DELAY_US (1000000/CLOCK_HZ)
#define DRAW_FLAG 3

extern struct timespec ts;

struct chip8
{
    unsigned char memory[MEM_SIZE];
    unsigned char V[16];
    unsigned short I;
    unsigned short PC;
    unsigned char SP;
    unsigned short stack[16];
    unsigned char DT;
    unsigned char ST;
    float last_decrement;
    float speed;
    unsigned char display_buf[VIDEO_WIDTH * VIDEO_HEIGHT];
    unsigned char keys[16];
};

void init(struct chip8 *c8);

int cycle(struct chip8 *c8);

#endif