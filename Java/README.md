# UnitTest library

## Content
This library allows you to run unit tests on your project. You will be able to check the good behaviour of your functions, by testing them one by one. 

This library gives you variable way to test your functions:
- `TEST(String method_name)` to run a classic test.
- `TEST(String method_name, long timeout)` to run a test with a timeout.
- `OTEST(String method_name, String pathToFileToCompare)` to compare standard output with the given file.
- `OTEST(String method_name, String pathToFileToCompare, long timeout)` to compare standard output with the given file with a timeout.

And variable `assert` functions:
- `assertArrayEquals(array1, array2)`: to verify if two arrays are equals.
- `assertEquals(var1, var2)`: to verify if two variables are equals.

They are compatible with every type. **But** if you compare objects you created, you
**should** have a `public boolean equals(Object myObject)`. If you do not implement it,
a default one will be given by Java. **But it probably not work as you want !**


A different version of this functions exists:
- `assertArrayEquals(String message, array1, array2)`
- `assertEquals(String message, var1, var2)`

The message will be displayed if the `assert*` function fail.

## Exemple of utilisation

```java
import static fr.its.unittest.ItsUnitTests.*;

public class MyTest
{
	public static void test1()
	{
	    ...
	}

	public static void test2()
	{
		...
	}

	public static void main(String[] args)
	{
		initItsTests(new MyTest());
		TEST("test2"); //Classic test
		TEST("test1", 5000); //Timeout test
		OTEST("test2", "test.txt"); //Test the output
		OTEST("test2", "test.txt", 300); //Timeout test the output
	}
}
```
An example is provided in this repository:
- go in the directory *Template*
- `make run` to compile and run the classic programm
- `make tests` to run the tests
- in the directory *tests*: `make run` to compile and run the test


## Advices

We advice you to create a **separate directory** for tests. In this directory, you will have your file(s) with the tests and a Makefile to run it easily (you can also use your principal Makefile and create a command `make test` that will do the same thing). It's better to have a `main` only for tests than to run them in your principal program. Like this, you will be able to split tasks if you work on team.

## Warnings

You **must not forget** the `import static fr.its.unittest.ItsUnitTests.*;` ! Every word is necessary
in this line !

Your main **must** begin with `initItsTests(new MyTest());` (or at least before calling `TEST` or `OTEST`)!


## Contributors
- [Hugo LAULLIER](https://github.com/HugoLaullier)
- [Thomas RIVES](https://github.com/ThomasRives)