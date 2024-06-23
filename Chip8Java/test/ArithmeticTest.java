import static org.junit.Assert.*;

public class ArithmeticTest
{
    @org.junit.Test
    public void testBytetoUInttoByte()
    {
        // Arrange
        byte b = -50;

        // Act
        int a = Byte.toUnsignedInt(b);

        // Arrange
        assertEquals(-50, (byte) a);
    }

    @org.junit.Test
    public void testInttoBytetoInt()
    {
        // Arrange
        int a = 247;

        // Act
        byte b = (byte) a;

        // Assert
        assertEquals(247, Byte.toUnsignedInt(b));
    }

    @org.junit.Test
    public void testIntToByteOverflow()
    {
        // Arrange
        int a = 255;

        // Act
        byte b = (byte) a;
        b += 1;

        // Assert
        assertEquals(0, Byte.toUnsignedInt(b));
    }
}
