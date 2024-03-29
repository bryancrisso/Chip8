public enum Instruction
{
    _0nnn, //jump to machine code instruction at nnn
    _00E0, //clear display
    _00EE, //Return from subroutine
    _1nnn, //Jump to location nnn
    _2nnn, //Call subroutine at nnn
    _3xkk, //Skip next instruction if Vx == kk
    _4xkk, //Skip next instruction if Vx != kk
    _5xy0, //Skip next instruction if Vx == Vy
    _6xkk, //Set Vx = kk
    _7xkk, //Set Vx = Vx + kk
    _8xy0, //Vx = Vy
    _8xy1, //Vx = Vx OR Vy
    _8xy2, //Vx = Vx AND Vy
    _8xy3, //Vx = Vx XOR Vy
    _8xy4, //Vx = Vx + Vy and set VF to 1 if there is an overflow
    _8xy5, //Vx = Vx - Vy and set VF to 1 if Vx > Vy
    _8xy6, //Logical shift Vx to the right. If the least significant bit of Vx is 1, set VF to 1
    _8xy7, //Vx = Vy - Vx and set VF to 1 if Vy > Vx
    _8xyE, //Logical shift Vx to the left. If the most significant bit of Vx is 1, set VF to 1.
    _9xy0, //Skip next instruction if Vx != Vy
    _Annn, //Set I = nnn
    _Bnnn, //Jump to location nnn + V0
    _Cxkk, //Set Vx = random byte AND kk
    _Dxyn, //Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision
    _Ex9E, //Skip next instruction if key with the value of Vx is pressed
    _ExA1, //Skip next instruction if key with the value of Vx is not pressed
    _Fx07, //Set Vx = delay timer value
    _Fx0A, //Wait for a key press, store the value of the key in Vx
    _Fx15, //Set delay timer = Vx
    _Fx18, //Set sound timer = Vx
    _Fx1E, //Set I = I + Vx
    _Fx29, //Set I = location of sprite for digit Vx
    _Fx33, //Store BCD representation of Vx in memory locations I, I+1, I+2
    _Fx55, //Store registers V0 through Vx in memory starting at location I
    _Fx65 //Read registers V0 through Vx from memory starting at location I
}