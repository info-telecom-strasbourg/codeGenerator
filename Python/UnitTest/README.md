# UnitTest library for python

## Content
This library allows you to run unit tests on your project. You will be able to check the good behaviour of your functions, by testing them one by one.

This library gives you variable way to check your test functions:
- `TEST(function, timeout=-1)` to run a classic test with an optionnal timeout (in milliseconds).
- `OTEST(function, expected_output_file)` to compare standard output with the given file with an optional timeout (in milliseconds).

In a test function, you can use these two functions :
- `assert(expression, message="")` to check that an expression is true. The message is an optionnal parameter, it will be displayed only if the test fail.
- `assert_file(first_file, second_file, message="") ` to check if first_file and second_file are identical. It can be text or binary files. The message is an optionnal parameter, it will be displayed only if the test fail.

**Warning** : assert and assert_file must be used in a test function checked by TEST or OTEST, otherwise it **won't work like you expect** and may cause unexpected errors!

## Display in the terminal
Here is what the tests will display in the terminal :
- If a test passes, it displays <span style="color: rgb(0,255,0)">Success</span> and the time taken by the function
- If in a test, an assertion is false, it displays <span style="color: rgb(255,0 ,0)">Failed</span> and exits the programm.
- If a test lasts longer than the timeout, it displays <span style="color: rgb(255,0 ,0)">Timeout</span>, the time out, and it exit the programm.
- If compared files aren't equal, it displays that the files are <span style="color: rgb(255,0 ,0)">different</span> and it will indicates the number of the line where the files are different.
- If you test the output, it displays that your expected_output_file is <span style="color: rgb(255,0 ,0)">different</span> from the real output. A *.log* file is created in order to see the real output, and understand the differences.


## Exemple of utilisation

```python
from unit_test import *
from time import sleep

# Test an equality
def test_equality():
    assert_true(1 == 1);

# Check that two strings are identical
def test_compare_string():
    string_1 = "hello";
    string_2 = "hello";
    assert_true(string_1 == string_1);

# A function that lasts 2 seconds
def test_timeout():
    sleep(2);

# A function that display messages on the terminal
def test_output():
    print("This is a message");
    print("This is an error message");

def test_output_and_timeout():
    print("This is a message");
    print("This is an error message");
    sleep(2);

# Check that two files are identical
def test_file_comparison():
    assert_file("output_1.txt", "output_2.txt");

# Test a wrong equality (this test have to fail)
def test_fail():
    assert_true(1 != 1);

print("--- Launch tests ---\n")
TEST(test_equality);
TEST(test_compare_string);
TEST(test_timeout, 2100);
OTEST(test_output, "output_1.txt");
OTEST(test_output_and_timeout, "output_1.txt", 2100);
TEST(test_file_comparison);
TEST(test_fail);
```

If you want to have an overview of how to use this library, you can take a look at the *Template* folder.

## Advices

We advice you to create a **separate directory** for tests. In this directory, you will have your file(s) with the tests and a Makefile to run it easily (you can also use your principal Makefile and create a command `make test` that will do the same thing). It's better to have a `main` only for tests than to run them in your principal program. Like this, you will be able to split tasks if you work on team.

## Warnings

`TEST`, `OTEST`, `assert`, `assert_file` are already used in this function ! Therefore, **Don't name your functions or variables like that!** If you do, it will lead to errors !

Also, don't use:
- `__its_unit_test_python_running`, `__its_stdout_redirection`, `__its_stderr_redirection`, `__its_process_func`, `__its_thread_load` : they are global variables used inside the library. **Do not use or change this variables**.
- `__its_loading_effect`, is a function used inside the library, **You should not use it**.

**If you decide to use them, it can lead to unexpected behavior.**

## Contributors
- [Hugo LAULLIER](https://github.com/HugoLaullier)
- [Thomas RIVES](https://github.com/ThomasRives)
