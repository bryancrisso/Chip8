/**
 * clear display
 * 00E0
 */
#define CLS 0

/**
 * Return from subroutine
 * 00EE
 */
#define RET 1

/**
 * jump to machine code instruction at nnn
 * 0nnn
 */
#define SYS 2

/**
 * Jump to location nnn
 * 1nnn
 */
#define JPn 3

/**
 * Call subroutine at nnn
 * 2nnn
 */
#define CALL 4

/**
 * Skip next instruction if Vx == kk
 * 3xkk
 */
#define SExb 5

/**
 * Skip next instruction if Vx != kk
 * 4xkk
 */
#define SNExb 6

/**
 * Skip next instruction if Vx == Vy
 * 5xy0
 */
#define SExy 7

/**
 * Set Vx = kk
 * 6xkk
 */
#define LDxb 8

/**
 * Set Vx = Vx + kk
 * 7xkk
 */
#define ADDxb 9

/**
 * Vx = Vy
 * 8xy0
 */
#define LDxy 10

/**
 * Vx = Vx OR Vy
 * 8xy1
 */
#define OR 11

/**
 * Vx = Vx AND Vy
 * 8xy2
 */
#define AND 12

/**
 * Vx = Vx XOR Vy
 * 8xy3
 */
#define XOR 13

/**
 * Vx = Vx + Vy and set VF to 1 if there is an overflow
 * 8xy4
 */
#define ADDxy 14

/**
 * Vx = Vx - Vy and set VF to 1 if Vx > Vy
 * 8xy5
 */
#define SUBxy 15

/**
 * Logical shift Vx to the right. If the least significant bit of Vx is 1, set VF to 1
 * 8xy6
 */
#define SHR 16

/**
 * Vx = Vy - Vx and set VF to 1 if Vy > Vx
 * 8xy7
 */
#define SUBN 17

/**
 * Logical shift Vx to the left. If the most significant bit of Vx is 1, set VF to 1.
 * 8xyE
 */
#define SHL 18

/**
 * Skip next instruction if Vx != Vy
 * 9xy0
 */
#define SNExy 19

/**
 * Set I = nnn
 * Annn
 */
#define LDI 20

/**
 * Jump to location nnn + V0
 * Bnnn
 */
#define JPVn 21

/**
 * Set Vx = random byte AND kk
 * Cxkk
 */
#define RND 22

/**
 * Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision
 * Dxyn
 */
#define DRW 23

/**
 * Skip next instruction if key with the value of Vx is pressed
 * Ex9E
 */
#define SKP 24

/**
 * Skip next instruction if key with the value of Vx is not pressed
 * ExA1
 */
#define SKNP 25

/**
 * Set Vx = delay timer value
 * Fx07
 */
#define LDxDT 26

/**
 * Wait for a key press, store the value of the key in Vx
 * Fx0A
 */
#define LDxK 27

/**
 * Set delay timer = Vx
 * Fx15
 */
#define LDDTx 28

/**
 * Set sound timer = Vx
 * Fx18
 */
#define LDST 29

/**
 * Set I = I + Vx
 * Fx1E
 */
#define ADDI 30

/**
 * Set I = location of sprite for digit Vx
 * Fx29
 */
#define LDF 31

/**
 * Store BCD representation of Vx in memory locations I, I+1, I+2
 * Fx33
 */
#define LDB 32

/**
 * Store registers V0 through Vx in memory starting at location I
 * Fx55
 */
#define LDIx 33

/**
 * Read registers V0 through Vx from memory starting at location I
 * Fx65
 */
#define LDxI 34