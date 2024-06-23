/**
 * Instructions that are used by Chip8
 */
public enum Instruction
{
    /**
     * clear display
     * 00E0
     */
    CLS,
    /**
     * Return from subroutine
     * 00EE
     */
    RET,
    /**
     * jump to machine code instruction at nnn
     * 0nnn
     */
    SYS,
    /**
     * Jump to location nnn
     * 1nnn
     */
    JPn,
    /**
     * Call subroutine at nnn
     * 2nnn
     */
    CALL,
    /**
     * Skip next instruction if Vx == kk
     * 3xkk
     */
    SExb,
    /**
     * Skip next instruction if Vx != kk
     * 4xkk
     */
    SNExb,
    /**
     * Skip next instruction if Vx == Vy
     * 5xy0
     */
    SExy,
    /**
     * Set Vx = kk
     * 6xkk
     */
    LDxb,
    /**
     * Set Vx = Vx + kk
     * 7xkk
     */
    ADDxb,
    /**
     * Vx = Vy
     * 8xy0
     */
    LDxy,
    /**
     * Vx = Vx OR Vy
     * 8xy1
     */
    OR,
    /**
     * Vx = Vx AND Vy
     * 8xy2
     */
    AND,
    /**
     * Vx = Vx XOR Vy
     * 8xy3
     */
    XOR,
    /**
     * Vx = Vx + Vy and set VF to 1 if there is an overflow
     * 8xy4
     */
    ADDxy,
    /**
     * Vx = Vx - Vy and set VF to 1 if Vx > Vy
     * 8xy5
     */
    SUBxy,
    /**
     * Logical shift Vx to the right. If the least significant bit of Vx is 1, set VF to 1
     * 8xy6
     */
    SHR,
    /**
     * Vx = Vy - Vx and set VF to 1 if Vy > Vx
     * 8xy7
     */
    SUBN,
    /**
     * Logical shift Vx to the left. If the most significant bit of Vx is 1, set VF to 1.
     * 8xyE
     */
    SHL,
    /**
     * Skip next instruction if Vx != Vy
     * 9xy0
     */
    SNExy,
    /**
     * Set I = nnn
     * Annn
     */
    LDI,
    /**
     * Jump to location nnn + V0
     * Bnnn
     */
    JPVn,
    /**
     * Set Vx = random byte AND kk
     * Cxkk
     */
    RND,
    /**
     * Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision
     * Dxyn
     */
    DRW,
    /**
     * Skip next instruction if key with the value of Vx is pressed
     * Ex9E
     */
    SKP,
    /**
     * Skip next instruction if key with the value of Vx is not pressed
     * ExA1
     */
    SKNP,
    /**
     * Set Vx = delay timer value
     * Fx07
     */
    LDxDT,
    /**
     * Wait for a key press, store the value of the key in Vx
     * Fx0A
     */
    LDxK,
    /**
     * Set delay timer = Vx
     * Fx15
     */
    LDDTx,
    /**
     * Set sound timer = Vx
     * Fx18
     */
    LDST,
    /**
     * Set I = I + Vx
     * Fx1E
     */
    ADDI,
    /**
     * Set I = location of sprite for digit Vx
     * Fx29
     */
    LDF,
    /**
     * Store BCD representation of Vx in memory locations I, I+1, I+2
     * Fx33
     */
    LDB,
    /**
     * Store registers V0 through Vx in memory starting at location I
     * Fx55
     */
    LDIx,
    /**
     * Read registers V0 through Vx from memory starting at location I
     * Fx65
     */
    LDxI
}