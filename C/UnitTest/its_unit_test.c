#include "its_unit_test.h"

/** A global variable indicating if a test is running */
static int test_running;

/** A global variable where stdout is saved */
static int saved_stdout;

/** A global variable where stderr is saved */
static int saved_stderr;

/** A global variable to store the thread used to display a loading effect */
static pthread_t loading_thread;

/** A global variable that store the thread used to launch the test function */
static pthread_t func_thread;


/**
 * @brief Display a loading effect (in the terminal)
 *
 * To stop the loading effect, you need to put test_running to 0
 * and join the thread. This function is specific to this library. Do not use
 * it outside of this library.
 * This function use two globals variables:
 * - test_running: to stop the loading effect.
 * - saved_stdout: the file descriptor to the terminal output.
 *
 * @param arg: useless, just to match with the prototype of a thread function.
 */
static void *
loading_threadingEffect(void *arg)
{
    (void)arg;
	int ind = 0;
	while (test_running)
	{
		switch (ind)
		{
		case 0:
			dprintf(saved_stdout, "   \b\b\b");
			break;
		case 1:
			dprintf(saved_stdout, ".  \b\b\b");
			break;
		case 2:
			dprintf(saved_stdout, ".. \b\b\b");
			break;
		case 3:
			dprintf(saved_stdout, "...\b\b\b");
			break;
		}
		ind = (ind + 1) % 4;
		usleep(300000);
	}
	dprintf(saved_stdout, "   \b\b\b");
	return NULL;
}

/**
 * @brief Check an expression and exit the programme if it is -1.
 *
 * It is used to check if a primitive system succeeded. It display an error
 * message if it failed.
 *
 * @param exp: the result of the primitive system.
 * @param msg: the message that will be displayed before exiting.
 */
static void
check(int exp, const char *msg, int red)
{
	if (exp == -1)
	{
	    if (red)
            dup2(saved_stderr, STDERR_FILENO);
		perror(msg);
		exit(EXIT_FAILURE);
	}
}

/**
 * @brief Check an expression and exit the programme if it is not 0.
 *
 * It is used to check if a pthread function succeeded. It display an error
 * message if it failed.
 * This function use one globals variables:
 * - saved_stderr: the terminal error output
 *
 * @param exp: the result of the pthread function.
 * @param msg: the message that will be displayed before exiting.
 */
static void
check_t(int exp, const char *msg)
{
	if(exp != 0)
	{
		dup2(saved_stderr, STDERR_FILENO);
		perror(msg);
		exit(EXIT_FAILURE);
	}
}

/**
 * @brief Stop the test after a certain amount of time. If so, the test will
 * be considered as failed.
 *
 * This function properly stop the running threads (loading thread, function
 * thread).
 * This function use three globals variables:
 * - test_running: : to stop the loading effect.
 * - loading_thread: the loading thread.
 * - saved_stderr: to display the timeout message.
 *
 * @param time: the maximum time, in milliseconds.
 * @param launch_func: the thread that execute the tested function.
 */
static void
launch_timeout(long int time)
{
    struct timeval start_time, cur_time;
    check(gettimeofday(&start_time, NULL), "gettimeofday failed", 1);
    while (test_running)
    {
        check(gettimeofday(&cur_time, NULL), "gettimeofday failed", 1);
        if (((cur_time.tv_sec * 1000000 + cur_time.tv_usec) -
            (start_time.tv_sec * 1000000 + start_time.tv_usec)) / 1000 > time)
        {
            test_running = 0;
            check_t(errno = pthread_join(loading_thread, NULL),
                    "Loading effect thread join failed");
            check_t(errno = pthread_detach(func_thread),
                    "Function launcher thread join failed");
            dprintf(saved_stderr, "%sTimeout (%ld ms)%s\n",
                    "\x1b[1;31m", time, "\x1b[0m");
            exit(EXIT_FAILURE);
        }
    }
    check_t(errno = pthread_join(func_thread, NULL),
            "Function launcher thread join failed");
}

static void *
thread_function(void *arg)
{
	void (*function)(void) = (void(*)()) arg;
	(*function)();
	test_running= 0;
	return NULL;
}

/**
 * @brief This function setup the environment for tests.
 *
 * stdout and stderr are redirected to not display the functions output and
 * keep a nice look for the tests outputs. It also launch the loading
 * effect function.
 * This function use three globals variables:
 * - test_running: : to stop the loading effect.
 * - loading_thread: the loading thread.
 * - saved_stdout: to save stdout.
 * - saved_stderr: to save stderr.
 *
 * @param test_name: the name of the function called for test.
 */
static void
setup_test(char *test_name)
{
    check(saved_stdout = dup(STDOUT_FILENO),
            "stdout redirection failed", 0);
    check(saved_stderr = dup(STDERR_FILENO),
            "stderr redirection failed", 0);
    fprintf(stdout, "Check %s : ", test_name);
    fflush(stdout);
    fflush(stderr);
    test_running = 1;
    check_t(errno = pthread_create(&loading_thread, NULL,
            loading_threadingEffect, NULL),
            "Loading effect thread creation failed");
}

/**
 * @brief Restore the environment and stop the thread displaying the loading
 * effect.
 *
 * It restore stdout and stderr. The thread displaying the loading effect is
 * stopped properly.
 *
 * @param start: a timeval structure containing the moment the test began.
 * @param end: a timeval structure containing the moment the test ended.
 * @param out: the redirected output.
 */
static void
end_test(struct timeval start, struct timeval end, int out)
{
    test_running = 0;
    check_t(errno = pthread_join(loading_thread, NULL),
            "Loading effect thread join failed");
    fflush(stdout);
    fflush(stderr);
    check(close(out), "Close of \"/dev/null\" failed", 1);
    check(dup2(saved_stdout, STDOUT_FILENO),
            "stdout redirection failed", 0);
    check(dup2(saved_stderr, STDERR_FILENO),
            "stderr redirection failed", 0);
    check(close(saved_stdout),
            "Close of saved_stdout failed", 0);
    fprintf(stdout, "%sSuccess%s (%ld ms)\n", "\033[0;32m", "\x1b[0m",
            ((end.tv_sec * 1000000 + end.tv_usec) -
            (start.tv_sec * 1000000 + start.tv_usec)) / 1000);
    fflush(stdout);
}

void
__test_classic_unittest_its(char *test_name, void (*function)(void))
{
    setup_test(test_name);
	int out;
	check(out = open("/dev/null", O_RDWR | O_APPEND),
	        "Open of \"/dev/null\" failed", 1);

	check(dup2(out, STDOUT_FILENO), "stdout redirection failed", 1);

	check(dup2(out, STDERR_FILENO), "stderr redirection failed", 1);

	struct timeval start, end;
	check(gettimeofday(&start, NULL), "gettimeofday failed", 1);
	(*function)();
	check(gettimeofday(&end, NULL), "gettimeofday failed", 1);
	end_test(start, end, out);
}

void
__test_timeout_unittest_its(char *test_name, void (*function)(void),
	unsigned long timeout)
{
    setup_test(test_name);
	int out = open("/dev/null", O_RDWR | O_APPEND);
	check(out, "Open of \"/dev/null\" failed", 1);
	check(dup2(out, STDOUT_FILENO), "stdout redirection failed", 1);
	check(dup2(out, STDERR_FILENO), "stderr redirection failed", 1);
	struct timeval start, end;
	check(gettimeofday(&start, NULL), "gettimeofday failed", 0);
	check_t(errno = pthread_create(&func_thread, NULL,thread_function, (void *)function), "Function launcher creation failed");
    launch_timeout(timeout);
    check(gettimeofday(&end, NULL), "gettimeofday failed", 1);
    end_test(start, end, out);
}

void
__test_output_unittest_its(char *test_name, void (*function)(void), char *expected_output_file)
{
    setup_test(test_name);
    int out = open("_its_out_test.log", O_RDWR | O_TRUNC | O_CREAT | O_APPEND, 0666);
    check(out, "Open of \"_its_out_test.log\" failed", 1);
    check(dup2(out, STDOUT_FILENO), "stdout redirection failed", 1);
    check(dup2(out, STDERR_FILENO), "stderr redirection failed", 1);

    struct timeval start, end;
    check(gettimeofday(&start, NULL), "gettimeofday failed", 1);
    (*function)();
    check(gettimeofday(&end, NULL), "gettimeofday failed", 1);
    end_test(start, end, out);
    test_running = 0;
    assert_file(expected_output_file, "_its_out_test.log");
    check(unlink("_its_out_test.log"), "Remove of \"_its_out_test.log\" failed", 0);
}

void
__test_output_timeout_unittest_its(char *test_name, void (*function)(void),
	char *expected_output_file, unsigned long timeout_millis)
{
    setup_test(test_name);
    int out = open("_its_out_test.log", O_RDWR | O_TRUNC | O_CREAT | O_APPEND,
            0666);
    check(out, "Open of \"_its_out_test.log\" failed", 1);
    check(dup2(out, STDOUT_FILENO), "stdout redirection failed", 1);
    check(dup2(out, STDERR_FILENO), "stderr redirection failed", 1);
    struct timeval start, end;
    check(gettimeofday(&start, NULL), "gettimeofday failed", 1);
    //launch thread function
    check_t(errno = pthread_create(&func_thread, NULL, thread_function, (void *)function), "Function launcher creation failed");
    launch_timeout(timeout_millis);
    check(gettimeofday(&end, NULL), "gettimeofday failed", 1);
    test_running = 0;
    assert_file(expected_output_file, "_its_out_test.log");
    end_test(start, end, out);
    check(unlink("_its_out_test.log"), "Remove of \"_its_out_test.log\" failed", 0);
}

void
__exit_test_unittest(char *test_name, void (*function)(void), int exit_code)
{
	setup_test(test_name);
	int out;
	int status;
	pid_t cpid;
	check(out = open("/dev/null", O_RDWR | O_APPEND),
	        "Open of \"/dev/null\" failed", 1);

	check(dup2(out, STDOUT_FILENO), "stdout redirection failed", 1);
	check(dup2(out, STDERR_FILENO), "stderr redirection failed", 1);

	struct timeval start, end;
	check(gettimeofday(&start, NULL), "gettimeofday failed", 1);

	switch (cpid = fork()) {
		case -1:
			check(-1, "fork failed", 1);
		case 0:
			(*function)();
			dup2(saved_stderr, STDERR_FILENO);
			dprintf(STDERR_FILENO, "Your function didn't called exit !\n");
			kill(getppid(), SIGKILL);
			exit(EXIT_FAILURE);
		default:
			break;
	}
	check(waitpid(cpid, &status, 0));
	if(WIFEXITED(status))
		assert(WEXITSTATUS(status) == exit_code);
	else
		__assert_unittest_its("Not finished with exit", 0);

	check(gettimeofday(&end, NULL), "gettimeofday failed", 1);
    end_test(start, end, out);
}

void
__assert_unittest_its(char *expression_text, int expression)
{
    if (!(expression))
    {
        test_running = 0;
        check_t(errno = pthread_join(loading_thread, NULL),
                "Loading effect thread join failed");
        dprintf(saved_stdout,
                    "%sFailed%s\nassertion failed : %s\n", "\x1b[1;31m",
                    "\x1b[0m",
                     expression_text);
        exit(EXIT_FAILURE);
    }
}

void
__assert_file_unittest_its(char *first_file, char *second_file)
{
        int file_origi = open(first_file, O_RDONLY);
        int file_saved = open(second_file, O_RDONLY);
        check(file_origi, "A problem occured when files were opened", 1);
        check(file_saved, "A problem occured when files were opened", 1);
        char buf_origi[1024] = "", buf_saved[1024] = "";
        int read_origi = 0, read_saved = 0;
        while ((read_origi = read(file_origi, buf_origi, 1024)) >= 0 &&
                (read_saved = read(file_saved, buf_saved, 1024)) > 0)
            if (read_origi != read_saved ||
                memcmp(buf_origi, buf_saved, read_saved))
            {
                test_running = 0;
                check_t(errno = pthread_join(loading_thread, NULL),
                        "Loading effect thread join failed");
                dprintf(saved_stdout, "%sFailed%s\nThe two files"
                        " are different\n", "\x1b[1;31m", "\x1b[0m");
                exit(EXIT_FAILURE);
            }
        check(read_origi, "An error occurred while reading files", 1);
        check(read_saved, "An error occurred while reading files", 1);
        check(close(file_saved), "Close of a file failed", 0);
        check(close(file_origi), "Close of a file failed", 0);
}
