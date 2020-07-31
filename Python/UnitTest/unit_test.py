import sys
import multiprocessing
from threading import Thread
from time import time, sleep

# A boolean that indicate if the test is running
__its_unit_test_python_running = False

__its_stdout_redirection = sys.stdout

__its_stderr_redirection = sys.stderr

__its_process_func = multiprocessing.Process()

__its_thread_load = Thread()

def TEST(func, timeout=-1):
	"""
	Test a function with our without Timeout
	The program will be exited if the test fail

	Parameters
	----------
	func: a function
		The function that's going to be tested

	timeout: long
		The maximum time given to the tested function
	"""
	global __its_unit_test_python_running
	global __its_stdout_redirection
	global __its_stderr_redirection
	global __its_thread_load
	print ("Check", func.__name__, ": ", end='')
	sys.stdout.flush()
	sys.stderr.flush()
	sys.stdout = open('/dev/null', 'w')
	sys.stderr = open('/dev/null', 'w')
	__its_unit_test_python_running = True
	if timeout == -1:
		__its_thread_load = Thread(target=__its_loading_effect)
		__its_thread_load.start()
		begin = time()
		func()
		elapsed_time = int((time() - begin)) * 1000
		__its_unit_test_python_running = False
		__its_thread_load.join()
	else:
		__its_thread_load = Thread(target=__its_loading_effect)
		__its_thread_load.start()
		__its_process_func = multiprocessing.Process(target=func)
		begin = time()
		__its_process_func.start()
		__its_process_func.join(timeout/1000)
		elapsed_time = int((time() - begin) * 1000)
		if __its_process_func.is_alive():
			__its_process_func.terminate()
		__its_unit_test_python_running = False
		__its_thread_load.join()
		if elapsed_time > timeout:
			print("\x1b[1;31mTimeout (", timeout," ms)\x1b[0m", file=__its_stdout_redirection, sep='')
			sys.exit(1)
	sys.stdout = __its_stdout_redirection
	sys.stderr = __its_stderr_redirection
	print("\033[0;32mSuccess\x1b[0m (",elapsed_time,"ms)", sep='')

def OTEST(func, expected_file, timeout=-1):
	"""
	Test a the output of a function with our without Timeout
	The program will be exited if the test fail

	Parameters
	----------
	func: a function
		The function that's going to be tested

	expected_file: string
		The name of the file that will be compared to the output

	timeout: long
		The maximum time given to the tested function
	"""
	global __its_unit_test_python_running
	global __its_stdout_redirection
	global __its_stderr_redirection
	global __its_process_func
	global __its_thread_load
	print ("Check", func.__name__, ": ", end='')
	sys.stdout.flush()
	sys.stderr.flush()
	output_file_name = func.__name__ + "__its_test.log"
	sys.stdout = open(output_file_name, 'w')
	sys.stderr = open(output_file_name, 'w')
	__its_unit_test_python_running = True
	if timeout == -1:
		__its_thread_load = Thread(target=__its_loading_effect)
		__its_thread_load.start()
		begin = time()
		func()
		elapsed_time = int((time() - begin)) * 1000
		__its_unit_test_python_running = False
		__its_thread_load.join()
		assert_file(expected_file, output_file_name)
	else:
		__its_thread_load = Thread(target=__its_loading_effect)
		__its_thread_load.start()
		__its_process_func = multiprocessing.Process(target=func)
		begin = time()
		__its_process_func.start()
		__its_process_func.join(timeout/1000)
		elapsed_time = int((time() - begin) * 1000)
		__its_process_func.terminate()
		__its_unit_test_python_running = False
		__its_thread_load.join()
		if elapsed_time > timeout:
			print("\x1b[1;31mTimeout (", timeout," ms)\x1b[0m", file=__its_stdout_redirection, sep='')
			sys.exit(1)
		assert_file(expected_file, output_file_name)
	sys.stdout = __its_stdout_redirection
	sys.stderr = __its_stderr_redirection
	print("\033[0;32mSuccess\x1b[0m (",elapsed_time,"ms)", sep='')

def __its_loading_effect():
	"""
	Create a loading effect
	"""
	global __its_unit_test_python_running
	global __its_stdout_redirection
	ind = 0
	__its_stdout_redirection.flush()

	while __its_unit_test_python_running:
		if ind == 0:
			print("   \b\b\b", file=__its_stdout_redirection, end='')
		elif ind == 1:
			print(".  \b\b\b", file=__its_stdout_redirection, end='')
		elif ind == 2:
			print(".. \b\b\b", file=__its_stdout_redirection, end='')
		else:
			print("...\b\b\b", file=__its_stdout_redirection, end='')
		__its_stdout_redirection.flush()
		ind = (ind+1)%4
		sleep(0.3)
	print("   \b\b\b", file=__its_stdout_redirection, end='')
	__its_stdout_redirection.flush()

def assert_file(expected_file, actual_file, message=""):
	"""
	Compare two files. If they are different, it exit the program

	Parameters
	----------
	expected_file: string
		The name of the expected file / the first file to compare

	actual_file: string
		The name of the actual file / the second file to compare

	message: string (optionnal)
		A message that will be printed in stderr if the test fail
	"""
	global __its_stderr_redirection
	global __its_process_func
	global __its_thread_load
	global __its_unit_test_python_running
	line_diff = 1
	with open(expected_file, 'r') as file1, open(actual_file, 'r') as file2:
		for line_file_1, line_file_2 in zip(file1, file2):
			if line_file_1 != line_file_2:
				if __its_process_func.is_alive():
					__its_process_func.terminate()
				__its_unit_test_python_running = False
				__its_thread_load.join()
				print("\x1b[1;31mFailed\x1b[0m", file=__its_stderr_redirection)
				print("Files are different at line ", line_diff, file=__its_stderr_redirection)
				if message:
					print(message,file=__its_stderr_redirection)
				sys.exit(1)
			line_diff += 1

def assert_true(condition, message=""):
	"""
	Check if the condition is true. If it is not, the programm will be exited

	Parameters
	----------
	condition: boolean
		The tested condition

	message: string (optionnal)
		A message that will be printed in stderr if the test fail
	"""
	global __its_stderr_redirection
	global __its_process_func
	global __its_thread_load
	global __its_unit_test_python_running
	if not condition:
		if __its_process_func.is_alive():
			__its_process_func.terminate()
		__its_unit_test_python_running = False
		__its_thread_load.join()
		print("\x1b[1;31mFailed\x1b[0m", file=__its_stderr_redirection)
		if message:
			print(message,file=__its_stderr_redirection)
		sys.exit(1)
