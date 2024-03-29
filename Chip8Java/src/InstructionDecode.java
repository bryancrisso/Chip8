public class InstructionDecode
{
    public static Instruction decodeInstruction(short instruction)
    {
        byte first = getNibble(instruction, 3);
        switch(Byte.toUnsignedInt(first))
        {
            case
        }
    }

    public static short getNNN(short instruction)
    {

    }

    public static byte getKK(short instruction)
    {

    }

    public static byte getNibble(short instruction, int index)
    {
        return (byte) ((Short.toUnsignedInt(instruction) & (0xF << index))>>index);
    }
}
