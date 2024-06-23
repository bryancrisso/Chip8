import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;

public class Loader
{
    public static byte[] readInstructions(String fname)
    {
        byte[] instructions = null;
        try
        {
            Path file = Paths.get(fname);
            instructions = Files.readAllBytes(file);
        } catch (IOException e)
        {
            System.err.println("Could not find the file mentioned!");
            e.printStackTrace();
        }
        return instructions;
    }
}