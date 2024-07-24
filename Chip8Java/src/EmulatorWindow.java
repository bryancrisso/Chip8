import java.awt.*;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.util.Map;

public class EmulatorWindow extends Frame implements KeyListener
{
    private final int[] displayBuffer;
    private final boolean[] keyState;
    private final Map<Integer, Integer> keyMap;
    private final int topOffset;
    private final int BUFFER_WIDTH;
    private final int BUFFER_HEIGHT;

    public EmulatorWindow(int[] _displayBuffer, int width, int height, int _BUFFER_WIDTH, int _BUFFER_HEIGHT, Map<Integer, Integer> _keyMap, boolean[] _keyState)
    {
        displayBuffer = _displayBuffer;
        BUFFER_WIDTH = _BUFFER_WIDTH;
        BUFFER_HEIGHT = _BUFFER_HEIGHT;
        keyMap = _keyMap;
        keyState = _keyState;

        setVisible(true);
        setSize(width, height);
        topOffset  = getInsets().top;
        setSize(width,height+topOffset);

        addWindowListener(new WindowAdapter()
        {
            public void windowClosing(WindowEvent e)
            {
                System.exit(0);
            }
        });

        addKeyListener(this);
    }

    public void paint (Graphics g)
    {
        for (int i = 0; i < displayBuffer.length; i++)
        {
            int x = i % BUFFER_WIDTH;
            int y = i / BUFFER_WIDTH;
            g.setColor(displayBuffer[i] == 0 ? Color.BLACK : Color.WHITE);
            g.fillRect(x*(getWidth()/BUFFER_WIDTH), y*((getHeight()-topOffset)/BUFFER_HEIGHT)+topOffset,
                    getWidth()/BUFFER_WIDTH, (getHeight()-topOffset)/BUFFER_HEIGHT);
        }
    }

    // methods for taking keyboard input

    @Override
    public void keyTyped(KeyEvent e)
    {
        // do nothing
    }

    @Override
    public void keyPressed(KeyEvent e)
    {
        if (keyMap.containsKey(e.getKeyCode()))
        {
            keyState[keyMap.get(e.getKeyCode())] = true;
        }
    }

    @Override
    public void keyReleased(KeyEvent e)
    {
        if (keyMap.containsKey(e.getKeyCode()))
        {
            keyState[keyMap.get(e.getKeyCode())] = false;
        }
    }
}
