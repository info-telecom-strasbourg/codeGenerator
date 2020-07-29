package fr.its.unittest;
import java.io.*;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.Arrays;

/**
 * A class used to test Java code
 * It contains multiples method "assert" and methods to launch different kind of tests
 *
 * @author ITS
 */
public class ItsUnitTests
{
	/**
	 * Colors for the console
	 */
	private static final String RESET = "\u001B[0m";
	private static final String RED = "\u001B[31m";
	private static final String GREEN = "\u001B[32m";

	/**
	 * The class that contains the tests
	 */
	private static Class<?> testClass;

	/** The object tested (used to get the class that contains the tests)
	 */
	private static java.lang.Object object;

	/**
	 * Indicate if the loading effect should be running
	 */
	private static boolean loading = false;

	/**
	 * Store the standard output to redirect it
	 */
	private static PrintStream console = System.out;

	/**
	 * Store the error output to redirect it
	 */
	private static PrintStream consoleErr = System.err;

	/**
	 * Used to redirect the standard output (the output of functions tested will not be displayed on the console)
	 */
	private static PrintStream devNull;

	/**
	 * The thread used to display a loading effect
	 */
	private static Thread loadingEffect;

	/**
	 * The method that contains the test
	 */
	private static Method testMethod = null;

	/**
	 * A thread to execute the function tested
	 */
	private static class FuncThread extends Thread
	{
		/**
		 * The function ran by the thread
		 */
		@Override
		public void run()
		{
			try
			{
				loadingEffect.start();
				testMethod.invoke(object);
				stopLoadingEffect();
			}
			catch (IllegalAccessException | InvocationTargetException e)
			{
				exitError("The method you are calling do not exist");
			}
		}
	}


	/**
	 * Initialize the variables used for tests
	 *
	 * @param obj the object tested (Object of the class of test)
	 */
	public static void initItsTests(java.lang.Object obj)
	{
		object = obj;
		testClass = obj.getClass();
		try { devNull = new PrintStream(new File("/dev/null")); }
		catch (FileNotFoundException e) { exitError("The redirection of the output failed."); }
	}

	/**
	 * Exit the program with an error message
	 *
	 * @param message The message displayed before exiting
	 */
	private static void exitError(String message)
	{
		consoleErr.println(message);
		System.exit(1);
	}

	/**
	 * Create a loading effect (to run in a different thread)
	 */
	private static void loadingEffect()
	{
		int ind = 0;
		long printTime = System.currentTimeMillis();
		console.print("   ");
		console.flush();
		while(loading)
		{
			if(System.currentTimeMillis() - printTime > 500)
			{
				switch(ind)
				{
					case 0:
						console.print("\b\b\b   ");
						break;
					case 1:
						console.print("\b\b\b.  ");
						break;
					case 2:
						console.print("\b\b\b.. ");
						break;
					case 3:
						console.print("\b\b\b...");
				}
				console.flush();
				ind = (ind+1)%4;
				printTime = System.currentTimeMillis();
			}
		}
		console.print("\b\b\b   \b\b\b");
		console.flush();
	}

	/**
	 * Stop properly the loading effect
	 */
	private static void stopLoadingEffect()
	{
		loading = false;
		try { loadingEffect.join(); }
		catch (InterruptedException e) { /* No problem if thread is interrupted*/ }
	}

	/**
	 * Initialise all useful variables for tests and display a message indicating which test will be run
	 *
	 * @param method The name of the method tested
	 */
	private static void initTest(String method)
	{
		console.flush();
		consoleErr.flush();
		console.print("Test of the function " + method + " : ");
		try { testMethod = testClass.getDeclaredMethod(method); }
		catch (NoSuchMethodException e) { exitError("The method you are calling do not exist"); }
		loadingEffect = new Thread() {public void run() { loadingEffect(); }};
		loading = true;
		System.setOut(devNull);
		System.setErr(devNull);
	}

	/**
	 * Run a test
	 *
	 * @return The elapsed time between the beginning of the test and the end of it
	 */
	private static long launchTest()
	{
		long start = System.currentTimeMillis();
		try { testMethod.invoke(object); }
		catch (IllegalAccessException | InvocationTargetException e)
		{
			exitError("An error occured when the method was invoked. It does not mean the test failed");
		}
		return System.currentTimeMillis() - start;
	}

	/**
	 * Test a method
	 *
	 * @param method The name of the method tested
	 */
	public static void TEST(String method)
	{
		initTest(method);

		loadingEffect.start();

		long ellapsedTime = launchTest();

		stopLoadingEffect();

		success(ellapsedTime);
	}

	/**
	 * Test a method with a timeout
	 *
	 * @param method The name of the method tested
	 * @param timeout The maximum time given to the test, in milliseconds
	 */
	public static void TEST(String method, long timeout)
	{
		initTest(method);

		long end = 0;
		Thread threadFunc = new FuncThread();

		long start = System.currentTimeMillis();
		threadFunc.start();

		try
		{
			threadFunc.join(timeout);
			end = System.currentTimeMillis();
			stopLoadingEffect();

			if(threadFunc.isAlive())
			{
				threadFunc.interrupt();
				threadFunc.join();
				timedout(timeout);
			}
		}
		catch (InterruptedException e) { /* No problem if interrupted */ }

		success(end - start);
	}

	/**
	 * Test the output of the test
	 *
	 * @param method The method tested
	 * @param expectedFileName The file with which the output will be compared
	 */
	public static void OTEST(String method, String expectedFileName)
	{
		initTest(method);

		File actualFile = new File(method + "__its_test.log");
		try { actualFile.createNewFile(); }
		catch (IOException e) { exitError("An error occured when the file was created"); }

		PrintStream outputFile = null;
		try { outputFile = new PrintStream(actualFile); }
		catch (FileNotFoundException e) { exitError("The redirection of the output failed."); }

		System.setOut(outputFile);
		System.setErr(outputFile);

		File expectedFile = new File(expectedFileName);

		loadingEffect.start();
		long elapsedTime = launchTest();
		stopLoadingEffect();

		System.setOut(devNull);
		System.setErr(devNull);

		boolean sameFiles = assertFile(actualFile.getAbsolutePath(), expectedFile.getAbsolutePath());
		actualFile.delete();

		if(sameFiles)
			success(elapsedTime);
	}

	/**
	 * Test the output of the test with a timeout
	 *
	 * @param method the tested method
	 * @param expectedFileName The file with which the output will be compared
	 * @param timeout The maximum time given to the test, in milliseconds
	 */
	public static void OTEST(String method, String expectedFileName, long timeout)
	{
		initTest(method);

		long end = 0;
		Thread threadFunc = new FuncThread();

		File actualFile = new File(method + "__its_test.log");
		PrintStream outputFile = null;

		try
		{
			actualFile.createNewFile();
			outputFile = new PrintStream(actualFile);
			System.setOut(outputFile);
			System.setErr(outputFile);
		}
		catch (IOException e) { exitError("An error occured when the file was created and the output redirected"); }

		File expectedFile = new File(expectedFileName);
		long start = System.currentTimeMillis();
		threadFunc.start();

		try
		{
			threadFunc.join(timeout);
			end = System.currentTimeMillis();
			stopLoadingEffect();
			if(threadFunc.isAlive())
			{
				threadFunc.interrupt();
				threadFunc.join();
				timedout(timeout);
			}
		}
		catch (InterruptedException e) { /* No problem if interrupted */ }
		System.setOut(devNull);
		System.setErr(devNull);
		boolean sameFiles = assertFile(actualFile.getAbsolutePath(), expectedFile.getAbsolutePath());
		actualFile.delete();

		if(sameFiles)
			success(end - start);
	}


	/**
	 * Get the absolute value of a double
	 *
	 * @param value The value you want to get the absolute value
	 * @return The absolute value of the given parameter
	 */
	private static double abs(double value) { return value > 0 ? value : -value; }

	/**
	 * Check if the two given files are identical or not, if they are not, it will exit the program with an error code
	 *
	 * @param expectedFileName The first file that will be compared
	 * @param actualFileName The second file that will be compared
	 * @return A boolean that indicate if the two files are identical
	 */
	public static boolean assertFile(String expectedFileName, String actualFileName)
	{
		BufferedReader reader1 = null, reader2 = null;
		String line1 = null, line2 = null;

		try
		{
			reader1 = new BufferedReader(new FileReader(expectedFileName));
			reader2 = new BufferedReader(new FileReader(actualFileName));
		}
		catch (FileNotFoundException e) { exitError("The file given does not exist"); }

		try
		{
			line1 = reader1.readLine();
			line2 = reader2.readLine();
		}
		catch (IOException e) { exitError("An error occured while reading a file"); }

		boolean areEqual = true;
		int lineNum = 1;

		while (line1 != null || line2 != null)
		{
			if(line1 == null || !line1.equals(line2))
			{
				areEqual = false;
				break;
			}

			try
			{
				line1 = reader1.readLine();
				line2 = reader2.readLine();
			}
			catch (IOException e) { exitError("An error occured while reading a file"); }
			lineNum++;
		}

		try
		{
			reader1.close();
			reader2.close();
		}
		catch (IOException e) { exitError("An error occured while closing files"); }

		if(!areEqual)
			failureFile(lineNum);

		return true;
	}


	/**
	 * Print a success message with the elapsed time between the beginning and the end of the execution of the function
	 * tested
	 *
	 * @param elapsedTime The elapsed time between the beginning and the end of the execution of the function tested
	 *                    in milliseconds
	 */
	private static void success(long elapsedTime)
	{
		console.println(GREEN + "Success" + RESET + " (" + elapsedTime + "ms)");
		System.setOut(console);
		System.setErr(consoleErr);
	}

	/**
	 * Print a failure message and exit the program with an error code
	 */
	private static void failure()
	{
		stopLoadingEffect();
		exitError(RED + "Failed" + RESET);
	}

	/**
	 * Print a failure message with the number of the first line where files are different and exit the program with
	 * an error code
	 */
	private static void failureFile(int lineNum)
	{
		stopLoadingEffect();
		exitError(RED + "Failed" + RESET + "\nFiles are different at line " + lineNum);
	}

	/**
	 * Print a personalized failure message and exit the program with an error code
	 */
	private static void failure(String message)
	{
		stopLoadingEffect();
		exitError(RED + "Failed" + RESET + "\n" + message);
	}

	/**
	 * Print a timeout message and exit the program with an error code
	 */
	private static void timedout(long maxTime)
	{
		stopLoadingEffect();
		exitError(RED + "Timeout" + RESET + " (" + maxTime + "ms)");
	}

	/**
	 * Test if two arrays of byte are equals. If they are not, it exit the program with an error code
	 *
	 * @param expected The expected array of byte
	 * @param actual The actual array of byte
	 */
	public static void assertArrayEquals(byte[] expected, byte[]actual)
	{
		if(!Arrays.equals(expected, actual))
			failure();
	}

	/**
	 * Test if two arrays of char are equals. If they are not, it exit the program with an error message
	 *
	 * @param expected The expected array of char
	 * @param actual The actual array of char
	 */
	public static void assertArrayEquals(char[] expected, char[]actual)
	{
		if(!Arrays.equals(expected, actual))
			failure();
	}

	/**
	 * Test if two arrays of int are equals. If they are not, it exit the program with an error message
	 *
	 * @param expected The expected array of int
	 * @param actual The actual array of int
	 */
	public static void assertArrayEquals(int[] expected, int[]actual)
	{
		if(!Arrays.equals(expected, actual))
			failure();
	}

	/**
	 * Test if two arrays of long are equals. If they are not, it exit the program with an error message
	 *
	 * @param expected The expected array of long
	 * @param actual The actual array of long
	 */
	public static void assertArrayEquals(long[] expected, long[]actual)
	{
		if(!Arrays.equals(expected, actual))
			failure();
	}

	/**
	 * Test if two arrays of object are equals. If they are not, it exit the program with an error message.
	 * If you are using an object you created, you must create an equals method with the signature:
	 * public boolean equals(MyObject myObject)
	 *
	 * @param expected The expected array of object
	 * @param actual The actual array of object
	 */
	public static void assertArrayEquals(java.lang.Object[] expected, java.lang.Object[]actual)
	{
		if(!Arrays.equals(expected, actual))
			failure();
	}

	/**
	 * Test if two arrays of short are equals. If they are not, it exit the program with an error message
	 *
	 * @param expected The expected array of short
	 * @param actual The actual array of short
	 */
	public static void assertArrayEquals(short[] expected, short[]actual)
	{
		if(!Arrays.equals(expected, actual))
			failure();
	}

	/**
	 * Test if two arrays of byte are equals. If they are not, it exit the program with an error message
	 *
	 * @param message The message that will be displayed in the failure message
	 * @param expected The expected array of byte
	 * @param actual The actual array of byte
	 */
	public static void assertArrayEquals(java.lang.String message, byte[] expected, byte[]actual)
	{
		if(!Arrays.equals(expected, actual))
			failure(message);
	}

	/**
	 * Test if two arrays of char are equals. If they are not, it exit the program with an error message
	 *
	 * @param message The message that will be displayed in the failure message
	 * @param expected The expected array of char
	 * @param actual The actual array of char
	 */
	public static void assertArrayEquals(java.lang.String message, char[] expected, char[]actual)
	{
		if(!Arrays.equals(expected, actual))
			failure(message);
	}

	/**
	 * Test if two arrays of int are equals. If they are not, it exit the program with an error message
	 *
	 * @param message The message that will be displayed in the failure message
	 * @param expected The expected array of int
	 * @param actual The actual array of int
	 */
	public static void assertArrayEquals(java.lang.String message, int[] expected, int[]actual)
	{
		if(!Arrays.equals(expected, actual))
			failure(message);
	}

	/**
	 * Test if two arrays of long are equals. If they are not, it exit the program with an error message
	 *
	 * @param message The message that will be displayed in the failure message
	 * @param expected The expected array of long
	 * @param actual The actual array of long
	 */
	public static void assertArrayEquals(java.lang.String message, long[] expected, long[]actual)
	{
		if(!Arrays.equals(expected, actual))
			failure(message);
	}

	/**
	 * Test if two arrays of object are equals. If they are not, it exit the program with an error message
	 * If you are using an object you created, you must create an equals method with the signature:
	 * public boolean equals(MyObject myObject)
	 *
	 * @param message The message that will be displayed in the failure message
	 * @param expected The expected array of Object
	 * @param actual The actual array of Object
	 */
	public static void assertArrayEquals(java.lang.String message, java.lang.Object[] expected, java.lang.Object[]actual)
	{
		if(!Arrays.equals(expected, actual))
			failure(message);
	}

	/**
	 * Test if two arrays of short are equals. If they are not, it exit the program with an error message
	 *
	 * @param message The message that will be displayed in the failure message
	 * @param expected The expected array of short
	 * @param actual The actual array of short
	 */
	public static void assertArrayEquals(java.lang.String message, short[] expected, short[]actual)
	{
		if(!Arrays.equals(expected, actual))
			failure(message);
	}

	/**
	 * Test if two double are identical. If they are not, it exit the program with an error message
	 *
	 * @param expected The expected double
	 * @param actual The actual double
	 */
	public static void assertEquals(double expected, double actual)
	{
		if(expected != actual)
			failure();
	}

	/**
	 * Test if two double are identical with a tolerance. If they are not, it exit the program with an error message
	 *
	 * @param expected The expected double
	 * @param actual The actual double
	 * @param delta The tolerance for the equality
	 */
	public static void assertEquals(double expected, double actual, double delta)
	{
		if(abs(expected - actual) < abs(delta))
			failure();
	}

	/**
	 * Test if two long are identical. If they are not, it exit the program with an error message
	 *
	 * @param expected The expected long
	 * @param actual The actual long
	 */
	public static void assertEquals(long expected, long actual)
	{
		if(expected != actual)
			failure();
	}

	/**
	 * Test if two Object are identical. If they are not, it exit the program with an error message
	 * If you are using an object you created, you must create an equals method with the signature:
	 * public boolean equals(MyObject myObject)
	 *
	 * @param expected The expected Object
	 * @param actual The actual Object
	 */
	public static void assertEquals(java.lang.Object expected, java.lang.Object actual)
	{
		if(!expected.equals(actual))
			failure();
	}


	/**
	 * Test if two double are identical. If they are not, it exit the program with an error message and an error code
	 *
	 * @param message The message that will be displayed in the failure message
	 * @param expected The expected double
	 * @param actual The actual double
	 */
	public static void assertEquals(java.lang.String message, double expected, double actual)
	{
		if(expected != actual)
			failure(message);
	}

	/**
	 * Test if two double are identical with a tolerance. If they are not, it exit the program with an error message
	 *  and an error code
	 *
	 * @param message The message that will be displayed in the failure message
	 * @param expected The expected double
	 * @param actual The actual double
	 * @param delta The tolerance for the equality
	 */
	public static void assertEquals(java.lang.String message, double expected, double actual, double delta)
	{
		if(abs(expected - actual) < abs(delta))
			failure(message);
	}

	/**
	 * Test if two long are identical. If they are not, it exit the program with an error message and an error code
	 *
	 * @param message The message that will be displayed in the failure message
	 * @param expected The expected long
	 * @param actual The actual long
	 */
	public static void assertEquals(java.lang.String message, long expected, long actual)
	{
		if(expected != actual)
			failure(message);
	}

	/**
	 * Test if two Object are identical. If they are not, it exit the program with an error message and an error code
	 *
	 * @param message The message that will be displayed in the failure message
	 * @param expected The expected object
	 * @param actual The actual object
	 */
	public static void assertEquals(java.lang.String message, java.lang.Object expected, java.lang.Object actual)
	{
		if(!expected.equals(actual))
			failure(message);
	}

	/**
	 * Test if the condition is false. If it is not, it exit the program with an error message and an error code
	 *
	 * @param condition The tested condition
	 */
	public static void assertFalse(boolean condition)
	{
		if(condition)
			failure();
	}

	/**
	 * Test if the condition is false. If it is not, it exit the program with an error message and an error code
	 *
	 * @param message The message that will be displayed in the failure message
	 * @param condition The tested condition
	 */
	public static void assertFalse(java.lang.String message, boolean condition)
	{
		if(condition)
			failure(message);
	}

	/**
	 * Test if the object is not null. If it is, it exit the program with an error message and an error code
	 *
	 * @param object The tested object
	 */
	public static void assertNotNull(java.lang.Object object)
	{
		if(object == null)
			failure();
	}

	/**
	 * Test if the object is not null. If it is, it exit the program with an error message and an error code
	 *
	 * @param message The message that will be displayed in the failure message
	 * @param object The tested condition
	 */
	public static void assertNotNull(java.lang.String message, java.lang.Object object)
	{
		if(object == null)
			failure(message);
	}

	/**
	 * Test if the objects are not the same. If they are, it exit the program with an error message and an error code
	 *
	 * @param unexpected The unexpected object
	 * @param actual The actual object
	 */
	public static void assertNotSame(java.lang.Object unexpected, java.lang.Object actual)
	{
		if(unexpected == actual)
			failure();
	}

	/**
	 * Test if the object are different. If they are not, it exit the program with an error message and an error code
	 *
	 * @param message The message that will be displayed in the failure message
	 * @param unexpected The unexpected object
	 * @param actual The actual object
	 */
	public static void assertNotSame(java.lang.String message, java.lang.Object unexpected, java.lang.Object actual)
	{
		if(unexpected == actual)
			failure(message);
	}

	/**
	 * Test if the given object is null. If it is not, it exit the program with an error message and an error code
	 *
	 * @param object The object tested
	 */
	public static void assertNull(java.lang.Object object)
	{
		if(object != null)
			failure();
	}

	/**
	 * Test if the given object is null. If it is not, it exit the program with an error message and an error code
	 *
	 * @param message The message that will be displayed in the failure message
	 * @param object The object tested
	 */
	public static void assertNull(java.lang.String message, java.lang.Object object)
	{
		if(object != null)
			failure(message);
	}

	/**
	 * Test if the object are the same. If they are not, it exit the program with an error message and an error code
	 *
	 * @param expected The unexpected object
	 * @param actual The actual object
	 */
	public static void assertSame(java.lang.Object expected, java.lang.Object actual)
	{
		if(expected != actual)
			failure();
	}

	/**
	 * Test if the object are the same. If they are not, it exit the program with an error message and an error code
	 *
	 * @param message The message that will be displayed in the failure message
	 * @param expected The unexpected object
	 * @param actual The actual object
	 */
	public static void assertSame(java.lang.String message, java.lang.Object expected, java.lang.Object actual)
	{
		if(expected != actual)
			failure(message);
	}

	/**
	 * Test if the given condition is true. If it is not, it exit the program with an error message and an error code
	 *
	 * @param condition The tested condition
	 */
	public static void assertTrue(boolean condition)
	{
		if(!condition)
			failure();
	}

	/**
	 * Test if the given condition is true. If it is not, it exit the program with an error message and an error code
	 *
	 * @param message The message that will be displayed in the failure message
	 * @param condition The tested condition
	 */
	public static void assertTrue(java.lang.String message, boolean condition)
	{
		if(!condition)
			failure(message);
	}
}