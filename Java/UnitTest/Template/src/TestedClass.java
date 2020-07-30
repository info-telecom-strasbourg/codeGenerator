import java.io.File;
import java.io.FileWriter;
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

    public static boolean equals(TestedClass expected, TestedClass actual)
    {
        return (expected.x == actual.x && expected.y == actual.y);
    }

    public static void sayHello()
    {
        System.out.println("Hello World!");
    }

    public static void sayHelloFile()
    {
        try 
		{
			File myFilew = new File("monFichierATest.txt");
			myFilew.createNewFile();
			FileWriter myFileww = new FileWriter(myFilew);
			myFileww.write("Hello World!\n");
            myFileww.close();
		} 
		catch (Exception e) { System.err.println("An error occured"); }
    }
}