import static org.junit.Assert.*;

public class LoaderTest
{
    @org.junit.Test
    public void readInstructions()
    {
        // Arrange
        String fname = "test.ch8";

        // Act
        byte[] instructions = Loader.readInstructions(fname);

        // Assert
        assertArrayEquals(new byte[]{32,0,48,0}, instructions);
    }
}