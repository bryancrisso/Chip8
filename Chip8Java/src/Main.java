public class Main
{
    public static void main(String[] args)
    {
        if (args.length == 0)
        {
            System.out.println("Usage: java Main <path to ROM> <Clock Speed=500>");
            System.exit(1);
        }
        new Chip8(args[0], args.length > 1 ? Double.parseDouble(args[1]) : 500d);
    }
}