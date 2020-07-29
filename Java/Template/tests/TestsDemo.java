import static fr.its.unittest.ItsUnitTests.*;
import java.util.ArrayList;

public class TestsDemo
{
	public static void test1()
	{
		TestedClass test = new TestedClass(3,4);

        assertEquals(3, test.getX());
        assertEquals(4, test.getY());
	}

	public static void test2()
	{
		TestedClass test = new TestedClass(3,4);
        test.addToList('a');
        ArrayList<Character> expected = new ArrayList<Character>();
        expected.add('a');
        assertEquals(expected, test.getList());
	}

	public static void test3()
	{
		TestedClass.sayHello();
	}

	public static void main(String[] args)
	{
		initItsTests(new TestsDemo());
		TEST("test1");
		TEST("test1", 200);
		TEST("test2");
	    TEST("test2", 20);
		OTEST("test3", "testHello.txt");
		OTEST("test3", "testHello.txt", 300);
	}
}