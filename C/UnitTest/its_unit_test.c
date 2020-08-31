#include "its_unit_test.h"


/**
 * @brief Display a loading effect (in the terminal)
 *
 * To stop the loading effect, you need to put __its_unit_test_c_running to 0
 * and join the thread. This function is specific to this library. Do not use
 * it outside of this library.
 * This function use two globals variables:
 * - __its_unit_test_c_running: to stop the loading effect.
 * - __its_unit_test_save_out: the file descriptor to the terminal output.
 *
 * @param arg: useless, just to match with the prototype of a thread function.
 */
static void *
__its_unit_test_c_loadingEffect(void *arg)
{
    (void)arg;
	int ind = 0;
	while (__its_unit_test_c_running)
	{
		switch (ind)
		{
		case 0:
			dprintf(__its_unit_test_save_out, "   \b\b\b");
			break;
		case 1:
			dprintf(__its_unit_test_save_out, ".  \b\b\b");
			break;
		case 2:
			dprintf(__its_unit_test_save_out, ".. \b\b\b");
			break;
		case 3:
			dprintf(__its_unit_test_save_out, "...\b\b\b");
			break;
		}
		ind = (ind + 1) % 4;
		usleep(300000);
	}
	dprintf(__its_unit_test_save_out, "   \b\b\b");
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
__check(int exp, const char *msg, int red)
{
	if (exp == -1)
	{
	    if (red)
            dup2(__its_unit_test_save_err, STDERR_FILENO);
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
 * - __its_unit_test_save_err: the terminal error output
 *
 * @param exp: the result of the pthread function.
 * @param msg: the message that will be displayed before exiting.
 */
static void
__check_t(int exp, const char *msg)
{
	if(exp != 0)
	{
		dup2(__its_unit_test_save_err, STDERR_FILENO);
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
 * - __its_unit_test_c_running: : to stop the loading effect.
 * - __its_unit_test_c_load: the loading thread.
 * - __its_unit_test_save_err: to display the timeout message.
 *
 * @param time: the maximum time, in milliseconds.
 * @param launch_func: the thread that execute the tested function.
 */
static void
__its_unit_test_c_timeout(long int time)
{
    struct timeval start_time, cur_time;
    __check(gettimeofday(&start_time, NULL), "gettimeofday failed", 1);
    while (__its_unit_test_c_running)
    {
        __check(gettimeofday(&cur_time, NULL), "gettimeofday failed", 1);
        if (((cur_time.tv_sec * 1000000 + cur_time.tv_usec) -
            (start_time.tv_sec * 1000000 + start_time.tv_usec)) / 1000 > time)
        {
            __its_unit_test_c_running = 0;
            __check_t(errno = pthread_join(__its_unit_test_c_load, NULL),
                    "Loading effect thread join failed");
            __check_t(errno = pthread_detach(__its_unit_test_c_func),
                    "Function launcher thread join failed");
            dprintf(__its_unit_test_save_err, "%sTimeout (%ld ms)%s\n",
                    "\x1b[1;31m", time, "\x1b[0m");
            exit(EXIT_FAILURE);
        }
    }
    __check_t(errno = pthread_join(__its_unit_test_c_func, NULL),
            "Function launcher thread join failed");
}

/**
 * @brief This function setup the environment for tests.
 *
 * stdout and stderr are redirected to not display the functions output and
 * keep a nice look for the tests outputs. It also launch the loading
 * effect function.
 * This function use three globals variables:
 * - __its_unit_test_c_running: : to stop the loading effect.
 * - __its_unit_test_c_load: the loading thread.
 * - __its_unit_test_save_out: to save stdout.
 * - __its_unit_test_save_err: to save stderr.
 *
 * @param test_name: the name of the function called for test.
 */
static void
setup_test(char *test_name)
{
    __check(__its_unit_test_save_out = dup(STDOUT_FILENO),
            "stdout redirection failed", 0);
    __check(__its_unit_test_save_err = dup(STDERR_FILENO),
            "stderr redirection failed", 0);
    fprintf(stdout, "Check %s : ", test_name);
    fflush(stdout);
    fflush(stderr);
    __its_unit_test_c_running = 1;
    __check_t(errno = pthread_create(&__its_unit_test_c_load, NULL,
            __its_unit_test_c_loadingEffect, NULL),
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
    __its_unit_test_c_running = 0;
    __check_t(errno = pthread_join(__its_unit_test_c_load, NULL),
            "Loading effect thread join failed");
    fflush(stdout);
    fflush(stderr);
    __check(close(out), "Close of \"/dev/null\" failed", 1);
    __check(dup2(__its_unit_test_save_out, STDOUT_FILENO),
            "stdout redirection failed", 0);
    __check(dup2(__its_unit_test_save_err, STDERR_FILENO),
            "stderr redirection failed", 0);
    __check(close(__its_unit_test_save_out),
            "Close of __its_unit_test_save_out failed", 0);
    fprintf(stdout, "%sSuccess%s (%ld ms)\n", "\033[0;32m", "\x1b[0m",
            ((end.tv_sec * 1000000 + end.tv_usec) -
            (start.tv_sec * 1000000 + start.tv_usec)) / 1000);
    fflush(stdout);
}

void
test1(char *test_name, void (*function)(void))
{
    setup_test(test_name);
    __with_timeout = 0;
	int out;
	__check(out = open("/dev/null", O_RDWR | O_APPEND),
	        "Open of \"/dev/null\" failed", 1);

	__check(dup2(out, STDOUT_FILENO), "stdout redirection failed", 1);

	__check(dup2(out, STDERR_FILENO), "stderr redirection failed", 1);

	struct timeval start, end;
	__check(gettimeofday(&start, NULL), "gettimeofday failed", 1);
	(*function)();
	__check(gettimeofday(&end, NULL), "gettimeofday failed", 1);
	end_test(start, end, out);
}

void
test2(char *test_name, void (*function)(void), unsigned long timeout)
{
    setup_test(test_name);
    __with_timeout = 1;
	int out = open("/dev/null", O_RDWR | O_APPEND);
	__check(out, "Open of \"/dev/null\" failed", 1);
	__check(dup2(out, STDOUT_FILENO), "stdout redirection failed", 1);
	__check(dup2(out, STDERR_FILENO), "stderr redirection failed", 1);
	struct timeval start, end;
	__check(gettimeofday(&start, NULL), "gettimeofday failed", 0);
	__check_t(errno = pthread_create(&__its_unit_test_c_func, NULL,
	        ({
	            void *callback()
	            {
	                (*function)();
	                __its_unit_test_c_running= 0;
	                return NULL;
	            }
	            callback;
	        }), NULL), "Function launcher creation failed");
    __its_unit_test_c_timeout(timeout);
    __check(gettimeofday(&end, NULL), "gettimeofday failed", 1);
    end_test(start, end, out);
}

void
test3(char *test_name, void (*function)(void), char *expected_output_file)
{
    setup_test(test_name);
    __with_timeout = 0;
    int out = open("_its_out_test.log", O_RDWR | O_TRUNC | O_CREAT | O_APPEND, 0666);
    __check(out, "Open of \"_its_out_test.log\" failed", 1);
    __check(dup2(out, STDOUT_FILENO), "stdout redirection failed", 1);
    __check(dup2(out, STDERR_FILENO), "stderr redirection failed", 1);

    struct timeval start, end;
    __check(gettimeofday(&start, NULL), "gettimeofday failed", 1);
    (*function)();
    __check(gettimeofday(&end, NULL), "gettimeofday failed", 1);
    end_test(start, end, out);
    __its_unit_test_c_running = 0;
    assert_file(expected_output_file, "_its_out_test.log");
    __check(unlink("_its_out_test.log"), "Remove of \"_its_out_test.log\" failed", 0);
}

void
test4(char *test_name, void (*function)(void), char *expected_output_file,
        unsigned long timeout_millis)
{
    setup_test(test_name);
    __with_timeout = 1;
    int out = open("_its_out_test.log", O_RDWR | O_TRUNC | O_CREAT | O_APPEND,
            0666);
    __check(out, "Open of \"_its_out_test.log\" failed", 1);
    __check(dup2(out, STDOUT_FILENO), "stdout redirection failed", 1);
    __check(dup2(out, STDERR_FILENO), "stderr redirection failed", 1);
    struct timeval start, end;
    __check(gettimeofday(&start, NULL), "gettimeofday failed", 1);
    //launch thread function
    __check_t(errno = pthread_create(&__its_unit_test_c_func, NULL,
            ({
                void *callback()
                {
                    (*function)();
                    __its_unit_test_c_running= 0;
                    return NULL;
                }
                callback;
            }), NULL), "Function launcher creation failed");
    __its_unit_test_c_timeout(timeout_millis);
    __check(gettimeofday(&end, NULL), "gettimeofday failed", 1);
    __its_unit_test_c_running = 0;
    assert_file(expected_output_file, "_its_out_test.log");
    end_test(start, end, out);
    __check(unlink("_its_out_test.log"), "Remove of \"_its_out_test.log\" failed", 0);
}

void
__its_assert(char* expression_text ,int expression)
{
    if (!(expression))
        {
            __its_unit_test_c_running = 0;
            __check_t(errno = pthread_join(__its_unit_test_c_load, NULL),
                    "Loading effect thread join failed");
            if (__with_timeout)
                __check_t(errno = pthread_detach(__its_unit_test_c_func),
                        "Function launcher thread join failed");
            dprintf(__its_unit_test_save_out,
                        "%sFailed%s\nassertion failed : %s\n", "\x1b[1;31m",
                        "\x1b[0m",
                         expression_text);
            exit(EXIT_FAILURE);
        }
}

void
__its_assert_file(char *first_file, char *second_file)
{
        int file_origi = open(first_file, O_RDONLY);
        int file_saved = open(second_file, O_RDONLY);
        __check(file_origi, "A problem occured when files were opened", 1);
        __check(file_saved, "A problem occured when files were opened", 1);
        char buf_origi[1024] = "", buf_saved[1024] = "";
        int read_origi = 0, read_saved = 0;
        while ((read_origi = read(file_origi, buf_origi, 1024)) >= 0 &&
                (read_saved = read(file_saved, buf_saved, 1024)) > 0)
            if (read_origi != read_saved ||
                memcmp(buf_origi, buf_saved, read_saved))
            {
                __its_unit_test_c_running = 0;
                __check_t(errno = pthread_join(__its_unit_test_c_load, NULL),
                        "Loading effect thread join failed");
                if (__with_timeout)
                    __check_t(errno = pthread_detach(__its_unit_test_c_func),
                            "Function launcher thread join failed");
                dprintf(__its_unit_test_save_out, "%sFailed%s\nThe two files"
                        " are different\n", "\x1b[1;31m", "\x1b[0m");
                exit(EXIT_FAILURE);
            }
        __check(read_origi, "An error occurred while reading files", 1);
        __check(read_saved, "An error occurred while reading files", 1);
        __check(close(file_saved), "Close of a file failed", 0);
        __check(close(file_origi), "Close of a file failed", 0);
}
