#include "its_unit_test.h"


/**
 * While the test is running, create a waiting animation
 */
static void *__its_unit_test_c_loadingEffect(void *arg)
{
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
}

static void
__check(int exp, const char *msg)
{
	if (exp == -1)
	{
		perror(msg);
		exit(EXIT_FAILURE);
	}
}

static void
__check_red(int exp, const char *msg)
{
	if (exp == -1)
	{
		dup2(__its_unit_test_save_err, STDERR_FILENO);
		perror(msg);
		exit(EXIT_FAILURE);
	}
}

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
 * Stop the programm if the test is not finished after a certain time
 * @param time: the maximum time, in milliseconds
 * @param launch_func: the thread that execute the tested function
 */
static void __its_unit_test_c_timeout(long int time, pthread_t *launch_func)
{
    struct timeval start_time, cur_time;
    __check_red(gettimeofday(&start_time, NULL), "gettimeofday failed");
    while (__its_unit_test_c_running)
    {
        __check_red(gettimeofday(&cur_time, NULL), "gettimeofday failed");
        if (((cur_time.tv_sec * 1000000 + cur_time.tv_usec) - (start_time.tv_sec * 1000000 + start_time.tv_usec)) / 1000 > time)
        {
            __its_unit_test_c_running = 0;
            dprintf(__its_unit_test_save_out, "%sTimeout (%ld ms)%s\n", "\x1b[1;31m", time, "\x1b[0m");
            __check_t(errno = pthread_detach(*launch_func), "Function launcher thread join failed");
            __check_t(errno = pthread_join(__its_unit_test_c_load, NULL), "Loading effect thread join failed");
            exit(EXIT_FAILURE);
        }
    }
    __check_t(errno = pthread_join(*launch_func, NULL), "Function launcher thread join failed");
}

void
test1(char *test_name, void (*function)(void))
{
	__check(__its_unit_test_save_out = dup(STDOUT_FILENO), "stdout redirection failed");
	__check(__its_unit_test_save_err = dup(STDERR_FILENO), "stderr redirection failed");
	fprintf(stdout, "Check %s : ", test_name);
	// empty the buffer of the different outputs before threads
	fflush(stdout);
	fflush(stderr);
	__its_unit_test_c_running = 1;
	__check_t(errno = pthread_create(&__its_unit_test_c_load, NULL, __its_unit_test_c_loadingEffect, NULL), "Loading effect thread creation failed");

	int out;
	__check_red(out = open("/dev/null", O_RDWR | O_APPEND),	"Open of \"/dev/null\" failed");

	__check_red(dup2(out, STDOUT_FILENO), "stdout redirection failed");

	__check_red(dup2(out, STDERR_FILENO), "stderr redirection failed");

	struct timeval start, end;
	__check_red(gettimeofday(&start, NULL), "gettimeofday failed");
	(*function)();
	__check_red(gettimeofday(&end, NULL), "gettimeofday failed");
	__its_unit_test_c_running = 0;

	__check_t(errno = pthread_join(__its_unit_test_c_load, NULL), "Loading effect thread join failed");
	fflush(stdout);
	fflush(stderr);
	__check_red(close(out), "Close of \"/dev/null\" failed");
	__check(dup2(__its_unit_test_save_out, STDOUT_FILENO), "stdout redirection failed");
	__check(dup2(__its_unit_test_save_err, STDERR_FILENO), "stderr redirection failed");
	__check(close(__its_unit_test_save_out), "Close of __its_unit_test_save_out failed");
	fprintf(stdout, "%sSuccess%s (%ld ms)\n", "\033[0;32m", "\x1b[0m", ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec)) / 1000);
	fflush(stdout);
}

void
test2(char *test_name, void (*function)(void), unsigned long timeout)
{
	__check(__its_unit_test_save_out = dup(STDOUT_FILENO), "stdout redirection failed");
	__check(__its_unit_test_save_err = dup(STDERR_FILENO), "stderr redirection failed");
	fprintf(stdout, "Check %s : ", test_name);
	fflush(stdout);
	fflush(stderr);
	__its_unit_test_c_running = 1;
	pthread_t load;
	__check_t(errno = pthread_create(&__its_unit_test_c_load, NULL, __its_unit_test_c_loadingEffect, NULL), "Loading effect thread creation failed");
	int out = open("/dev/null", O_RDWR | O_APPEND);
	__check_red(out, "Open of \"/dev/null\" failed");
	__check_red(dup2(out, STDOUT_FILENO), "stdout redirection failed");
	__check_red(dup2(out, STDERR_FILENO), "stderr redirection failed");
	struct timeval start, end;
	__check(gettimeofday(&start, NULL), "gettimeofday failed");
	pthread_t launch_func;
	//launch thread function
	__check_t(errno = pthread_create(&launch_func, NULL, ({void *callback() {(*function)(); __its_unit_test_c_running= 0;} callback; }), NULL), "Function launcher creation failed");
    __its_unit_test_c_timeout((timeout), &launch_func);
    __check_red(gettimeofday(&end, NULL), "gettimeofday failed");
    __its_unit_test_c_running = 0;
    __check_t(pthread_join(__its_unit_test_c_load, NULL), "Loading effect thread join failed");
    __check_red(close(out), "Close of \"/dev/null\" failed");
    __check(dup2(__its_unit_test_save_out, STDOUT_FILENO), "stdout redirection failed");
    __check(dup2(__its_unit_test_save_err, STDERR_FILENO), "stderr redirection failed");
    __check(close(__its_unit_test_save_out), "Close of save_out failed");
    fprintf(stdout, "%sSuccess%s (%ld ms)\n", "\033[0;32m", "\x1b[0m", ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec)) / 1000);
    fflush(stdout);
}

void
test3(char *test_name, void (*function)(void), char *expected_output_file)
{
    __check(__its_unit_test_save_out = dup(STDOUT_FILENO), "stdout redirection failed");
    __check(__its_unit_test_save_err = dup(STDERR_FILENO), "stderr redirection failed");
    fprintf(stdout, "Check %s : ", test_name);
    fflush(stdout);
    fflush(stderr);
    __its_unit_test_c_running = 1;
    __check_t(pthread_create(&__its_unit_test_c_load, NULL, __its_unit_test_c_loadingEffect, NULL), "Loading effect thread creation failed");
    int out = open("_its_out_test.log", O_RDWR | O_TRUNC | O_CREAT | O_APPEND, 0666);
    __check_red(out, "Open of \"_its_out_test.log\" failed");
    __check_red(dup2(out, STDOUT_FILENO), "stdout redirection failed");
    __check_red(dup2(out, STDERR_FILENO), "stderr redirection failed");

    struct timeval start, end;
    __check_red(gettimeofday(&start, NULL), "gettimeofday failed");
    (*function)();
    __check_red(gettimeofday(&end, NULL), "gettimeofday failed");
    __its_unit_test_c_running = 0;
    assert_file(expected_output_file, "_its_out_test.log");
    __check_t(pthread_join(__its_unit_test_c_load, NULL), "Loading effect thread join failed");
    fflush(stdout);
    fflush(stderr);
    __check_red(close(out), "Close of \"/dev/null\" failed");
    __check(dup2(__its_unit_test_save_out, STDOUT_FILENO), "stdout redirection failed");
    __check(dup2(__its_unit_test_save_err, STDERR_FILENO), "stderr redirection failed");
    __check(close(__its_unit_test_save_out), "Close of __its_unit_test_save_out failed");
    fprintf(stdout, "%sSuccess%s (%ld ms)\n", "\033[0;32m", "\x1b[0m", ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec)) / 1000);
    fflush(stdout);
    __check(unlink("_its_out_test.log"), "Remove of \"_its_out_test.log\" failed");
}

void
test4(char *test_name, void (*function)(void), char *expected_output_file, unsigned long timeout_millis)
{
    __check(__its_unit_test_save_out = dup(STDOUT_FILENO), "stdout redirection failed");
    __check(__its_unit_test_save_err = dup(STDERR_FILENO), "stderr redirection failed");
    fprintf(stdout, "Check %s : ", test_name);
    fflush(stdout);
    fflush(stderr);
    __its_unit_test_c_running = 1;
    __check_t(pthread_create(&__its_unit_test_c_load, NULL, __its_unit_test_c_loadingEffect, NULL), "Loading effect thread creation failed");
    int out = open("_its_out_test.log", O_RDWR | O_TRUNC | O_CREAT | O_APPEND, 0666);
    __check_red(out, "Open of \"_its_out_test.log\" failed");
    __check_red(dup2(out, STDOUT_FILENO), "stdout redirection failed");
    __check_red(dup2(out, STDERR_FILENO), "stderr redirection failed");
    struct timeval start, end;
    __check_red(gettimeofday(&start, NULL), "gettimeofday failed");
    pthread_t launch_func;
    //launch thread function
    __check_t(errno = pthread_create(&launch_func, NULL, ({void *callback() {(*function)(); __its_unit_test_c_running= 0;} callback; }), NULL), "Function launcher creation failed");
    __its_unit_test_c_timeout((timeout_millis), &launch_func);
    __check_red(gettimeofday(&end, NULL), "gettimeofday failed");
    __its_unit_test_c_running = 0;
    assert_file(expected_output_file, "_its_out_test.log");
    __check_t(pthread_join(__its_unit_test_c_load, NULL), "Loading effect thread join failed");
    fflush(stdout);
    fflush(stderr);
    __check_red(close(out), "Close of \"/dev/null\" failed");
    __check(dup2(__its_unit_test_save_out, STDOUT_FILENO), "stdout redirection failed");
    __check(dup2(__its_unit_test_save_err, STDERR_FILENO), "stderr redirection failed");
    __check(close(__its_unit_test_save_out), "Close of __its_unit_test_save_out failed");
    fprintf(stdout, "%sSuccess%s (%ld ms)\n", "\033[0;32m", "\x1b[0m", ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec)) / 1000);
    fflush(stdout);
    __check(unlink("_its_out_test.log"), "Remove of \"_its_out_test.log\" failed");
}
