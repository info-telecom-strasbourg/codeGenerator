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

# A function that display messages on the terminal and lasts 2 seconds
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
