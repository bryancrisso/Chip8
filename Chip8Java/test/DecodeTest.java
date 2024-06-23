import org.junit.Test;

import java.util.Arrays;

import static org.junit.Assert.*;

public class DecodeTest
{
    @Test
    public void decodeAll()
    {
        // Arrange
        String fname = "test2.ch8";
        String[] expectedInstructions = Arrays.stream(Instruction.values()).map(Instruction::toString).toArray(String[]::new);

        // Act
        byte[] instructions = Loader.readInstructions(fname);

        for (int i = 0; i < instructions.length; i+=2)
        {
            short ins = 0;
            ins = (short) (ins | Byte.toUnsignedInt(instructions[i]) << 8);
            ins = (short) (ins | Byte.toUnsignedInt(instructions[i+1]));
            try
            {
//                System.out.println(InsDecode.decodeInstruction(ins) + " " + Integer.toHexString(ins));
                assertEquals(expectedInstructions[i/2], InsDecode.decodeInstruction(ins).toString());
            }
            catch (UnknownInstructionError e)
            {
                e.printStackTrace();
            }
        }
    }
}
