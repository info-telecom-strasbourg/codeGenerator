# UnitTest library for C

## Content
This library allows you to run unit tests on your project. You will be able to check the good behaviour of your functions, by testing them one by one.

This library gives you variable way to check your test functions:
- `TEST(function)` to run a classic test.
- `TEST(function, timeout_millis)` to run a test with a timeout (in milliseconds).
- `OTEST(function, expected_output_file)` to compare standard output with the given file.
- `OTEST(function, expected_output_file, timeout_millis)` to compare standard output with the given file with a timeout (in milliseconds).
- `ETEST(function, exit_code)` to check if your function exit with the correct exit code.

In a test function, you can use these two functions :
- `assert(expression)` to check that an expression is true
- `assert_file(first_file, second_file) ` to check if first_file and second_file are identical. It can be text or binary files.

To simulate an allocation error or a primitive system error, you can give `__remaining_alloc_its`, `__remaining_primsys_its` and `__remaining_threads_fct_its` a positive value (the number of successful l allocation / PS). You can give a different value in each test function. If you do not give a value a value at the beginning of your test, they will be put to `-1` and all allocations/PS/threads will succeed. 

**Warning** : assert and assert_file must be used in a test function checked by TEST or OTEST, otherwise it won't work! Also, you must have the two global variables `__remaining_alloc_its`, `__remaining_primsys_its` and `__remaining_threads_fct_its` declared.

## Display in the terminal
Here is what the tests will display in the terminal :
- If a test passes, it displays <span style="color: rgb(0,255,0)">Success</span> and the time taken by the function
- If in a test, an assertion is false, it displays <span style="color: rgb(255,0 ,0)">Failed</span> and exits the programm.
- If a test lasts longer than the timeout, it displays <span style="color: rgb(255,0 ,0)">Timeout</span>, the time out, and it exit the programm.
- If compared files aren't equal, it displays that the files are <span style="color: rgb(255,0 ,0)">different</span>.
- If you test the output, it displays that your expected_output_file is <span style="color: rgb(255,0 ,0)">different</span> from the real output. A *.log* file is created in order to see the real output, and understand the differences.


## Example of utilisation

```cpp
long long __remaining_alloc_its = -1; // if you do not want the library to make your allocation fail
long long __remaining_primsys_its = -1: // if you do not want the library to make your primitive system fail
long long __remaining_threads_fct_its = -1; // if you do not want the library to maye your threads function fail

void myTestFunction()
{
    ...
	__remaining_alloc_its = 3; //__
    assert(expr); // Test an expression
    assert_file(file_1, file_2); // Check if two files are identical
    ...
}

int main(void)
{
    ...
    // Test myTestFunction
    TEST(myTestFunction);

    // Test myTestFunction with a timeout (here, the function must finish in 1 second maximum)
    TEST(myTestFunction, 1000);

    // Test myTestFunction, and check if the output is identical of the output_expected file
    OTEST(myTestFunction, output_exected);

    // Test myTestFunction with a timeout (here, the function must finish in 1 second maximum), and check if the output is identical of the output_expected file
    OTEST(myTestFunction, output_exected, 1000);
    ...
}
```
If you want to have an overview of how to use this library, you can take a look at the *Example* folder.

## Advices

We advice you to create a **separate directory** for tests. In this directory, you will have your file(s) with the tests and a Makefile to run it easily (you can also use your principal Makefile and create a command `make test` that will do the same thing). It's better to have a `main` only for tests than to run them in your principal program. Like this, you will be able to split tasks if you work on team.
We also advice you to reinitialize `__remaining_alloc_its` and `__remaining_primsys_its` in each test function. It's a global variable and you must no forget that if it is set to 3, it will be modified and will be decreased. If you do not reset it, it can lead to errors.

## Warnings

It is possible that your IDE detect with the auto-compilation that a `;` is missing. Don't worry, it's due to the override of macros, it will still work! This problem do not appear with classic compilation.

In the *Makefile*, use -pthread, otherwise the libray won't work... (you can understand how to use -pthread in the *Makefile* of the *Example* folder)

`TEST`, `OTEST`, `assert`, `assert_file` are macros ! Therefore, **Don't name your functions, class, variables or macros like that!** If you do, it will lead to errors because the macro will replace your function, class or variable !

Also, don't use:
- `__ITS_TEST_1`, `__ITS_TEST_2`, `__ITS_TEST_3`, `__ITS_TEST_4`, `__ITS_GET_MACRO_TEST` and `__ITS_GET_MACRO_OTEST` : they are macros used inside the library.
- `__test_classic_unittest_its`, `__test_timeout_unittest_its`, `__test_output_unittest_its`, `__test_output_timeout_unittest_its`, `__assert_unittest_its`, and `__assert_file_unittest_its` are functions used inside the library. They are equivalent to the macros. We advise you to use the macros because it is easier and reduce your code.

**If you decide to use them, it can lead to unexpected behavior.**

Finally, if you use *valgrind* on the tests and a test <span style="color: red">don't success</span>, this library will generate a memory leak. If a test <span style="color: green">succeed</span>, no memory leaks will be generated by this library. Moreover, *valgrind* significantly reduces the speed of execution of your functions, then all the timout test will be wrong. **Therefore, we recommand you to use *valgrind* only if you don't use timeout tests, and if all your tests are successfull**.

## Example
You can run the example by using `make` to compile and `./test` to run the tests or `make run` to compile and run the tests.

## Contributors
- [Hugo LAULLIER](https://github.com/HugoLaullier)
- [Thomas RIVES](https://github.com/ThomasRives)
