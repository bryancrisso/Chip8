public class InsDecode
{
    public static Instruction decodeInstruction(short instruction) throws UnknownInstructionError
    {
        byte first = getNibble(instruction, 3);
        return switch(Byte.toUnsignedInt(first))
        {
            case 0 -> switch (instruction & 0xFFF)
                {
                    case 0x0E0 -> Instruction.CLS;
                    case 0x0EE -> Instruction.RET;
                    default -> Instruction.SYS;
                };
            case 1 -> Instruction.JPn;
            case 2 -> Instruction.CALL;
            case 3 -> Instruction.SExb;
            case 4 -> Instruction.SNExb;
            case 5 -> Instruction.SExy;
            case 6 -> Instruction.LDxb;
            case 7 -> Instruction.ADDxb;

            case 8 -> switch (instruction & 0xF)
            {
                    case 0 -> Instruction.LDxy;
                    case 1 -> Instruction.OR;
                    case 2 -> Instruction.AND;
                    case 3 -> Instruction.XOR;
                    case 4 -> Instruction.ADDxy;
                    case 5 -> Instruction.SUBxy;
                    case 6 -> Instruction.SHR;
                    case 7 -> Instruction.SUBN;
                    case 0xE -> Instruction.SHL;
                    default -> throw new UnknownInstructionError("Unknown instruction code: " + instruction);
                };
            case 0x9 -> Instruction.SNExy;
            case 0xA -> Instruction.LDI;
            case 0xB -> Instruction.JPVn;
            case 0xC -> Instruction.RND;
            case 0xD -> Instruction.DRW;
            case 0xE -> switch (instruction & 0xF)
                {
                    case 1 -> Instruction.SKNP;
                    case 0xE -> Instruction.SKP;
                    default -> throw new UnknownInstructionError("Unknown instruction code: " + instruction);
                };
            case 0xF -> switch (instruction & 0xFF)
                {
                    case 0x7 -> Instruction.LDxDT;
                    case 0x0A -> Instruction.LDxK;
                    case 0x15 -> Instruction.LDDTx;
                    case 0x18 -> Instruction.LDST;
                    case 0x1E -> Instruction.ADDI;
                    case 0x29 -> Instruction.LDF;
                    case 0x33 -> Instruction.LDB;
                    case 0x55 -> Instruction.LDIx;
                    case 0x65 -> Instruction.LDxI;
                    default -> throw new UnknownInstructionError("Unknown instruction code: " + instruction);
                };
            default -> throw new UnknownInstructionError("Unknown instruction code: " + instruction);
        };
    }

    // This doesn't need casting to unsigned integer - no chance of it ever being negative
    public static short getNNN(short instruction)
    {
        return (short)(instruction & 0xFFF);
    }

    // Cast to unsigned integer
    public static byte getKK(short instruction)
    {
        return (byte)(instruction&0xFF);
    }

    // This doesn't need casting to unsigned integer - no chance of it ever being negative
    public static byte getNibble(short instruction, int index)
    {
        index = index * 4;
        return (byte) ((Short.toUnsignedInt(instruction) & (0xF << index))>>index);
    }
}
