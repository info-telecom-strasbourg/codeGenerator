import static fr.its.unittest.ItsUnitTests.*;

import java.io.File;
import java.util.ArrayList;

public class TestsDemo
{
	public static void test1()
	{
		TestedClass test1[] = {new TestedClass(3,4), new TestedClass(3,5)};
		TestedClass test2[] = {new TestedClass(3,4), new TestedClass(3,5)};
		assertArrayEquals(test1,test2);
		assertEquals(test1[1].getX(), test2[1].getX());
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

	public static void test4()
	{
		TestedClass.sayHelloFile();
		assertFile("testHello.txt", "monFichierATest.txt");
		File createdFile = new File("monFichierATest.txt");
		createdFile.delete();
	}

	public static void main(String[] args)
	{
		TestedClass test = new TestedClass(3, 4);
		TestedClass test2 = new TestedClass(3, 4);
		initItsTests(new TestsDemo());
		TEST("test1");
		TEST("test1", 200);
		TEST("test2");
		TEST("test2", 200);
		OTEST("test3", "testHello.txt");
		OTEST("test3", "testHello.txt", 300);
		TEST("test4");
	    TEST("test4", 20);
	}
}
