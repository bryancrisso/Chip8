#include <time.h>

#define MEM_SIZE 4096
#define VIDEO_WIDTH 64
#define VIDEO_HEIGHT 32
#define CLOCK_HZ 500
#define DELAY_MS (1/CLOCK_HZ * 1000)

extern unsigned char memory[MEM_SIZE];

struct timespec ts = {
    .tv_sec = DELAY_MS / 1000,
    .tv_nsec = (DELAY_MS % 1000) / 1000000
};