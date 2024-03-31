public class Chip8
{
    private final byte[] registers = new byte[16];
    private short I;
    private byte DT;
    private byte ST;
    private short PC;
    private byte SP;
    private final short[] stack = new short[16];
    private final byte[] memory = new byte[4096];


    public void init()
    {
        // Setup memory
        String romFname = "";
        byte[] instructions = Loader.readInstructions(romFname);

        System.arraycopy(instructions, 0, memory, 512, instructions.length);

        // Add the hexadecimal digits to the interpreter space

        // Start an output window

        // Start an input method

        // Fetch-Decode-Execute Cycle

        // Exit
    }

    public void loop()
    {

    }

    public short fetch()
    {
        int ins = 0;
        ins = ins | Byte.toUnsignedInt(memory[PC])<<8;
        ins = ins | Byte.toUnsignedInt(memory[PC+1]);
        return (short) ins;
    }

    public void execute(short instruction, Instruction type)
    {
        switch (type)
        {

        }
    }
}
