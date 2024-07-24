import java.util.Arrays;
import java.util.HashMap;
import java.util.Map;
import java.util.Random;
import java.awt.event.KeyEvent;

public class Chip8
{
    private final byte[] V = new byte[16];
    private short I;
    private short PC;
    private byte SP = 0;
    private final short[] stack = new short[16];

    private byte DT;
    private byte ST;
    // last time since we clock cycled
    private double lastDecrement = 0;
    // CPU clock speed in Hz
    private final double speed;

    private final byte[] memory = new byte[4096];

    private final String romFname;
    private final Random rand;

    private final int VIDEO_HEIGHT = 32;
    private final int VIDEO_WIDTH = 64;
    private final int[] displayBuffer = new int[VIDEO_WIDTH*VIDEO_HEIGHT];

    private final boolean[] keys = new boolean[16];
    private final Map<Integer, Integer> keymap = new HashMap<>();

    public Chip8(String romName, double clockSpeed)
    {
        romFname = romName;
        rand = new Random();
        speed = clockSpeed;
        init();
    }

    public void init()
    {

        // Setup memory
        byte[] instructions = Loader.readInstructions(romFname);

        System.arraycopy(instructions, 0, memory, 512, instructions.length);

        PC = 512;

        // Add the hexadecimal digits to the interpreter space

        byte[] hexCodes = new byte[]
                {
                        (byte) 0xF0, (byte) 0x90, (byte) 0x90, (byte) 0x90, (byte) 0xF0, // 0
                        (byte) 0x20, (byte) 0x60, (byte) 0x20, (byte) 0x20, (byte) 0x70, // 1
                        (byte) 0xF0, (byte) 0x10, (byte) 0xF0, (byte) 0x80, (byte) 0xF0, // 2
                        (byte) 0xF0, (byte) 0x10, (byte) 0xF0, (byte) 0x10, (byte) 0xF0, // 3
                        (byte) 0x90, (byte) 0x90, (byte) 0xF0, (byte) 0x10, (byte) 0x10, // 4
                        (byte) 0xF0, (byte) 0x80, (byte) 0xF0, (byte) 0x10, (byte) 0xF0, // 5
                        (byte) 0xF0, (byte) 0x80, (byte) 0xF0, (byte) 0x90, (byte) 0xF0, // 6
                        (byte) 0xF0, (byte) 0x10, (byte) 0x20, (byte) 0x40, (byte) 0x40, // 7
                        (byte) 0xF0, (byte) 0x90, (byte) 0xF0, (byte) 0x90, (byte) 0xF0, // 8
                        (byte) 0xF0, (byte) 0x90, (byte) 0xF0, (byte) 0x10, (byte) 0xF0, // 9
                        (byte) 0xF0, (byte) 0x90, (byte) 0xF0, (byte) 0x90, (byte) 0x90, // A
                        (byte) 0xE0, (byte) 0x90, (byte) 0xE0, (byte) 0x90, (byte) 0xE0, // B
                        (byte) 0xF0, (byte) 0x80, (byte) 0x80, (byte) 0x80, (byte) 0xF0, // C
                        (byte) 0xE0, (byte) 0x90, (byte) 0x90, (byte) 0x90, (byte) 0xE0, // D
                        (byte) 0xF0, (byte) 0x80, (byte) 0xF0, (byte) 0x80, (byte) 0xF0, // E
                        (byte) 0xF0, (byte) 0x80, (byte) 0xF0, (byte) 0x80, (byte) 0x80  // F
                };

        System.arraycopy(hexCodes, 0, memory, 0, hexCodes.length);

        // Populate keymap

        keymap.put(KeyEvent.VK_1, 0x1);
        keymap.put(KeyEvent.VK_2, 0x2);
        keymap.put(KeyEvent.VK_3, 0x3);
        keymap.put(KeyEvent.VK_4, 0xC);
        keymap.put(KeyEvent.VK_Q, 0x4);
        keymap.put(KeyEvent.VK_W, 0x5);
        keymap.put(KeyEvent.VK_E, 0x6);
        keymap.put(KeyEvent.VK_R, 0xD);
        keymap.put(KeyEvent.VK_A, 0x7);
        keymap.put(KeyEvent.VK_S, 0x8);
        keymap.put(KeyEvent.VK_D, 0x9);
        keymap.put(KeyEvent.VK_F, 0xE);
        keymap.put(KeyEvent.VK_Z, 0xA);
        keymap.put(KeyEvent.VK_X, 0x0);
        keymap.put(KeyEvent.VK_C, 0xB);
        keymap.put(KeyEvent.VK_V, 0xF);

        // Start an output window

        EmulatorWindow window = new EmulatorWindow(displayBuffer, 640, 320, 64, 32, keymap, keys);
        window.paint(window.getGraphics());

        // Fetch-Decode-Execute Cycle

        while (PC < memory.length-1)
        {
            // making sure that we are running at the correct speed
            double currTime = System.currentTimeMillis();
            double elapsedTime = currTime - lastDecrement;
            if (elapsedTime > (1/speed) * 1000)
            {
                cycle();

                // paint graphics to screen
                window.paint(window.getGraphics());

                lastDecrement = currTime;
            }
        }

        // Exit
        System.out.println("End");
    }

    public void cycle()
    {

            // one fetch decode execute loop

            short ins = fetch();

            PC += 2;

            try
            {
                execute(ins, InsDecode.decodeInstruction(ins));
            }
            catch (UnknownInstructionError e)
            {
                e.printStackTrace();
            }
            // decrement sound and delay timers
            if (DT > 0) DT--;
            if (ST > 0) {ST--; System.out.println("Beep");}
    }

    public short fetch()
    {
        int ins = 0;
        ins = ins | Byte.toUnsignedInt(memory[PC])<<8;
        ins = ins | Byte.toUnsignedInt(memory[PC+1]);
        return (short) ins;
    }

    public void execute(short ins, Instruction type)
    {
        // TO THINK ABOUT - overflows and whatnot - tbh shouldn't be an issue if we got it all stored in appropriate containers
        byte x = InsDecode.getNibble(ins,2);
        byte y = InsDecode.getNibble(ins,1);
        byte kk = InsDecode.getKK(ins);
        short nnn = InsDecode.getNNN(ins);
        switch (type)
        {
            case SYS -> System.out.println();
            case CLS -> Arrays.fill(displayBuffer, 0);
            case RET -> {PC = stack[Byte.toUnsignedInt(SP)]; SP--;}
            case JPn -> PC = nnn;
            // what to do if SP overflows?? ¯\_(ツ)_/¯ programmer's issue
            case CALL -> {SP++; stack[Byte.toUnsignedInt(SP)] = PC; PC = nnn;}
            case SExb -> {if (V[x] == kk) PC += 2;}
            case SNExb -> {if (V[x] != kk) PC += 2;}
            case SExy -> {if (V[x] == V[y]) PC += 2;}
            case LDxb -> V[x] = kk;
            case ADDxb -> V[x] += kk;
            case LDxy -> V[x] = V[y];
            case OR -> V[x] = (byte) (V[x] | V[y]);
            case AND -> V[x] = (byte) (V[x] & V[y]);
            case XOR -> V[x] = (byte) (V[x] ^ V[y]);
            case ADDxy -> {
                if (Byte.toUnsignedInt(V[x]) + Byte.toUnsignedInt(V[y]) > 255)
                {
                    V[0xF] = 1;
                }
                else
                {
                    V[0xF] = 0;
                }
                V[x] += V[y];
            }
            case SUBxy -> {
                if (V[x] > V[y])
                {
                    V[0xF] = 1;
                }
                else
                {
                    V[0xF] = 0;
                }
                V[x] -= V[y];
            }
            case SHR ->
            {
                if ((V[x] & 0b1) == 0b1)
                {
                    V[0xF] = 1;
                }
                else
                {
                    V[0xF] = 0;
                }
                V[x] = (byte) (V[x] >> 1);
            }
            case SUBN -> {
                if (V[y] > V[x])
                {
                    V[0xF] = 1;
                }
                else
                {
                    V[0xF] = 0;
                }
                V[y] -= V[x];
            }
            case SHL ->
            {
                if ((V[x] & -128) == -128)
                {
                    V[0xF] = 1;
                }
                else
                {
                    V[0xF] = 0;
                }
                V[x] = (byte) (V[x] << 1);
            }
            case SNExy -> {if (V[x] != V[y]) PC += 2;}
            case LDI -> I = nnn;
            case JPVn -> PC = (short) (V[0] + nnn);
            case RND -> V[x] = (byte) (rand.nextInt(256) & kk);
            case DRW ->
            {
                byte height = InsDecode.getNibble(ins, 0);
                int xPos = Byte.toUnsignedInt(V[x]) % VIDEO_WIDTH;
                int yPos = Byte.toUnsignedInt(V[y]) % VIDEO_HEIGHT;

                V[0xF] = 0;

                for (int row = 0; row < height; row++)
                {
                    byte spriteRow = memory[I + row];

                    for (int col = 0; col < 8; col++)
                    {
                        byte spritePixel = (byte)(spriteRow & (0x80 >> col));

                        // make sure to mod it so it wraps around
                        int bufferPos = ((yPos + row) * VIDEO_WIDTH + (xPos + col)) % displayBuffer.length;

                        int screenPixel = displayBuffer[bufferPos];

                        if (Byte.toUnsignedInt(spritePixel) > 0)
                        {
                            if (screenPixel == 1)
                            {
                                V[0xF] = 1;
                            }
                            displayBuffer[bufferPos] ^= 1;
                        }
                    }
                }
            }
            case SKP -> {if (keys[Byte.toUnsignedInt(V[x])]) PC += 2;}
            case SKNP -> {if (!keys[Byte.toUnsignedInt(V[x])]) PC += 2;}
            case LDxDT -> V[x] = DT;
            case LDxK -> {
                boolean keyPress = false;
                for (int i = 0; i < keys.length; i++)
                {
                    if (keys[i])
                    {
                        V[x] = (byte) i;
                        keyPress = true;
                    }
                }
                if (!keyPress)
                {
                    PC -= 2;
                }
            }
            case LDDTx -> DT = V[x];
            case LDST -> ST = V[x];
            case ADDI -> I += V[x];
            case LDF -> I = (short)(V[x] * 5);
            case LDB ->
            {
                memory[I] = (byte) ((Byte.toUnsignedInt(V[x]) / 100) % 10);
                memory[I+1] = (byte) ((Byte.toUnsignedInt(V[x]) / 10) % 10);
                memory[I+2] = (byte) (Byte.toUnsignedInt(V[x]) % 10);
            }
            case LDIx ->
            {
                if (x + 1 >= 0) System.arraycopy(V, 0, memory, I, x + 1);
            }
            case LDxI ->
            {
                if (x + 1 >= 0) System.arraycopy(memory, I, V, 0, x + 1);
            }
        }
    }
}
