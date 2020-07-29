import java.util.ArrayList;

public class TestedClass
{
    private int x;
    private int y;
    private ArrayList<Character> list = new ArrayList<Character>();

    TestedClass(int x, int y)
    {
        this.x = x;
        this.y = y;
    }

    public int getX()
    {
        return x;
    }

    public int getY()
    {
        return y;
    }

    public void addToList(Character c)
    {
        list.add(c);
    }

    public ArrayList<Character> getList()
    {
        return list;
    }

    public static void sayHello()
    {
        System.out.println("Hello World!");
    }
}