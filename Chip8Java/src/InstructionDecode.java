public class InstructionDecode
{
    public static Instruction decodeInstruction(short instruction) throws UnknownInstructionError
    {
        byte first = getNibble(instruction, 0);
        switch(Byte.toUnsignedInt(first))
        {
            case 0:

                if ((instruction & 0xFFF) == 0x0E0)
                {
                    return Instruction.CLS;
                }
                else if ((instruction & 0xFFF) == 0x0EE)
                {
                    return Instruction.RET;
                }
                else
                {
                    return Instruction.SYS;
                }

            case 1:
                return Instruction.JPn;

            case 2:
                return Instruction.CALL;

            case 3:
                return Instruction.SExb;

            case 4:
                return Instruction.SNExb;

            case 5:
                return Instruction.SExy;

            case 6:
                return Instruction.LDxb;

            case 7:
                return Instruction.ADDxb;

            case 8:
                switch (instruction & 0xF)
                {
                    case 0:
                        return Instruction.LDxy;

                    case 1:
                        return Instruction.OR;

                    case 2:
                        return Instruction.AND;

                    case 3:
                        return Instruction.XOR;

                    case 4:
                        return Instruction.ADDxy;

                    case 5:
                        return Instruction.SUBxy;

                    case 6:
                        return Instruction.SHR;

                    case 7:
                        return Instruction.SUBN;

                    case 0xE:
                        return Instruction.SHL;
                }
                break;
            case 0x9:
                return Instruction.SNExy;
            case 0xA:
                return Instruction.LDI;
            case 0xB:
                return Instruction.JPVn;
            case 0xC:
                return Instruction.RND;
            case 0xD:
                return Instruction.DRW;
            case 0xE:
                switch (instruction & 0xF)
                {
                    case 1:
                        return Instruction.SKNP;
                    case 0xE:
                        return Instruction.SKP;
                }
                break;
            case 0xF:
                switch (instruction & 0xFF)
                {
                    case 0x7:
                        return Instruction.LDxDT;
                    case 0x0A:
                        return Instruction.LDKx;
                    case 0x15:
                        return Instruction.LDDTx;
                    case 0x18:
                        return Instruction.LDST;
                    case 0x1E:
                        return Instruction.ADDI;
                    case 0x29:
                        return Instruction.LDF;
                    case 0x33:
                        return Instruction.LDB;
                    case 0x55:
                        return Instruction.LDIx;
                    case 0x65:
                        return Instruction.LDxI;
                }
                break;
        }
        throw new UnknownInstructionError();
    }

    public static short getNNN(short instruction)
    {
        return (short)(instruction & 0xFFF);
    }

    public static byte getKK(short instruction)
    {
        return (byte)(instruction&0xFF);
    }

    public static byte getNibble(short instruction, int index)
    {
        return (byte) ((Short.toUnsignedInt(instruction) & (0xF << index))>>index);
    }
}
