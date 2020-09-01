#include "its_unit_test.h"
#include <iostream>

/* A boolean that indicate if the test is running */
static bool __its_unit_test_cpp_running;

/* The thread that display the loading effect */
static std::thread __its_unit_test_cpp_load;

/* A stream buffer to store the cout stream */
static std::streambuf *__its_unit_test_stream_buffer_cout;

/* A stream buffer to store the cerr stream */
static std::streambuf *__its_unit_test_stream_buffer_cerr;

/**
 * While the test is running, create a waiting animation
 */
static void __its_unit_test_cpp_loadingEffect()
{
    try
    {
        std::ostream stream_err(__its_unit_test_stream_buffer_cerr);
        int ind = 0;
        while (__its_unit_test_cpp_running)
        {
            switch (ind)
            {
                case 0:
                    stream_err << "   \b\b\b";
                    break;
                case 1:
                    stream_err << ".  \b\b\b";
                    break;
                case 2:
                    stream_err << ".. \b\b\b";
                    break;
                case 3:
                    stream_err << "...\b\b\b";
                    break;
            }
            stream_err.flush();
            ind = (ind + 1) % 4;
            usleep(300000);
        }
        stream_err << "   \b\b\b";
        stream_err.flush();
    }
    catch (std::exception const &e)
    {
        std::cerr.rdbuf(__its_unit_test_stream_buffer_cerr);
        std::cerr << "Error : " << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }
}

/**
 * Stop the programm if the test is not finished after a certain time
 * @param time: the maximum time, in milliseconds
 * @param launch_func: the thread that execute the tested function
 */
static void __its_unit_test_cpp_timeout(float time, std::thread &launch_func)
{
    try
    {
        std::ostream stream_err(__its_unit_test_stream_buffer_cerr);

        auto start_time = std::chrono::steady_clock::now();
        while (__its_unit_test_cpp_running)
            if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start_time).count() > time)
            {
                __its_unit_test_cpp_running = false;
                stream_err << "\x1b[1;31m"
                           << "Timeout (" << time << " ms)"
                           << "\x1b[0m" << std::endl;
                stream_err.flush();
                launch_func.detach();
                __its_unit_test_cpp_load.join();
                exit(EXIT_FAILURE);
            }
        launch_func.join();
    }
    catch (std::exception const &e)
    {
        std::cerr.rdbuf(__its_unit_test_stream_buffer_cerr);
        std::cerr << "Error : " << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }
}

void
__its_assert(std::string expression_text, bool expression)
{
    try
    {
        if (!(expression))
        {
            std::ostream stream_err(__its_unit_test_stream_buffer_cerr);
            __its_unit_test_cpp_running = false;
            __its_unit_test_cpp_load.join();
            stream_err << "\x1b[1;31m"
                       << "Failed "
                       << "\x1b[0m" << std::endl
                       << "assertion failed : " << expression_text << std::endl;
            stream_err.flush();
            exit(EXIT_FAILURE);
        }
    }
    catch (std::exception const &e)
    {
        std::cerr.rdbuf(__its_unit_test_stream_buffer_cerr);
        std::cout << "Error : " << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }
}

void
__its_assert_files(std::string first_file, std::string second_file)
{
    try
    {
        std::ostream stream_err(__its_unit_test_stream_buffer_cerr);
        bool equal = false;
        std::ifstream f1(first_file, std::ifstream::binary | std::ifstream::ate);
        if (f1.fail())
        {
            __its_unit_test_cpp_running = false;
            __its_unit_test_cpp_load.join();
            stream_err << "\x1b[1;31m"
                       << "Failed "
                       << "\x1b[0m" << std::endl
                       << "Failure when opening the file " << first_file << "!" << std::endl;
            stream_err.flush();
            exit(EXIT_FAILURE);
        }
        std::ifstream f2(second_file, std::ifstream::binary | std::ifstream::ate);
        if (f2.fail())
        {
            __its_unit_test_cpp_running = false;
            __its_unit_test_cpp_load.join();
            stream_err << "\x1b[1;31m"
                       << "Failed "
                       << "\x1b[0m" << std::endl
                       << "Failure when opening the file " << second_file << "!" << std::endl;
            stream_err.flush();
            exit(EXIT_FAILURE);
        }
        if (f1.tellg() == f2.tellg())
        {
            f1.seekg(0, std::ifstream::beg);
            f2.seekg(0, std::ifstream::beg);
            equal = std::equal(std::istreambuf_iterator<char>(f1.rdbuf()),
                               std::istreambuf_iterator<char>(),
                               std::istreambuf_iterator<char>(f2.rdbuf()));
        }
        f1.close();
        f2.close();
        if (!(equal))
        {
            __its_unit_test_cpp_running = false;
            __its_unit_test_cpp_load.join();
            stream_err << "\x1b[1;31m"
                       << "Failed "
                       << "\x1b[0m" << std::endl
                       << "The files \"" << first_file << "\" and \"" << second_file << "\" are different!" << std::endl;
            stream_err.flush();
            exit(EXIT_FAILURE);
        }
    }
    catch (std::exception const &e)
    {
        std::cerr.rdbuf(__its_unit_test_stream_buffer_cerr);
        std::cerr << "Error : " << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }
}


void
test1(std::string __current_test_name, void (*function)(void))
{
	try
	{
		__its_unit_test_stream_buffer_cout = std::cout.rdbuf();
		__its_unit_test_stream_buffer_cerr = std::cerr.rdbuf();
		std::cout << "Check " << __current_test_name << " : ";
		fflush(stdout);
		fflush(stderr);
		__its_unit_test_cpp_running = true;
		__its_unit_test_cpp_load = std::thread(__its_unit_test_cpp_loadingEffect);
		std::ofstream file("/dev/null");
		std::cout.rdbuf(file.rdbuf());
		std::cerr.rdbuf(file.rdbuf());
		auto start = std::chrono::steady_clock::now();
		function();
		auto end = std::chrono::steady_clock::now();
		std::cout.rdbuf(__its_unit_test_stream_buffer_cout);
		std::cerr.rdbuf(__its_unit_test_stream_buffer_cerr);
		unsigned long elapsed_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
		__its_unit_test_cpp_running = false;
		__its_unit_test_cpp_load.join();
		std::cout << "\033[0;32m"
				  << "Success"
				  << "\x1b[0m"
				  << " ("
				  << (unsigned int)elapsed_time / 1000 << "ms)" << std::endl;
		fflush(stdout);
		file.close();
	}
	catch (std::exception const &e)
	{
		std::cerr.rdbuf(__its_unit_test_stream_buffer_cerr);
		std::cerr << "Error : " << e.what() << std::endl;
		exit(EXIT_FAILURE);
	}
}

void
test2(std::string __current_test_name, void (*function)(void), unsigned long timeout_millis)
{
	try
	{
		__its_unit_test_stream_buffer_cout = std::cout.rdbuf();
		std::cout << "Check " << __current_test_name << " : ";
		fflush(stdout);
		fflush(stderr);
		__its_unit_test_cpp_running = true;
		__its_unit_test_cpp_load = std::thread(__its_unit_test_cpp_loadingEffect);
		std::ofstream file("/dev/null");
		std::streambuf *__its_unit_test_stream_buffer_cerr = std::cerr.rdbuf();
		std::cout.rdbuf(file.rdbuf());
		std::cerr.rdbuf(file.rdbuf());
		auto start = std::chrono::steady_clock::now();
		std::thread launch_func = std::thread([function]() {function(); __its_unit_test_cpp_running= false; });
		__its_unit_test_cpp_timeout((timeout_millis), launch_func);
		auto end = std::chrono::steady_clock::now();
		__its_unit_test_cpp_load.join();
		unsigned long elapsed_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
		std::cout.rdbuf(__its_unit_test_stream_buffer_cout);
		std::cerr.rdbuf(__its_unit_test_stream_buffer_cerr);
		std::cout << "\033[0;32m"
				  << "Success "
				  << "\x1b[0m"
				  << " (" << (unsigned int)elapsed_time / 1000 << "ms)" << std::endl;
		fflush(stdout);
		file.close();
	}
	catch (std::exception const &e)
	{
		std::cerr.rdbuf(__its_unit_test_stream_buffer_cerr);
		std::cerr << "Error : " << e.what() << std::endl;
		exit(EXIT_FAILURE);
	}
}

void
test3(std::string __current_test_name, void (*function)(void), std::string expected_output_file)
{
	try
	{
		__its_unit_test_stream_buffer_cout = std::cout.rdbuf();
		__its_unit_test_stream_buffer_cerr = std::cerr.rdbuf();
		std::cout << "Check " << __current_test_name << " : ";
		fflush(stdout);
		fflush(stderr);
		__its_unit_test_cpp_running = true;
		__its_unit_test_cpp_load = std::thread(__its_unit_test_cpp_loadingEffect);
		std::ofstream file(__current_test_name + std::string("_its_test.log"));
		std::cout.rdbuf(file.rdbuf());
		std::cerr.rdbuf(file.rdbuf());
		auto start = std::chrono::steady_clock::now();
		function();
		auto end = std::chrono::steady_clock::now();
		__its_assert_files(expected_output_file, __current_test_name + std::string("_its_test.log"));
		std::cout.rdbuf(__its_unit_test_stream_buffer_cout);
		std::cerr.rdbuf(__its_unit_test_stream_buffer_cerr);
		unsigned long elapsed_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
		__its_unit_test_cpp_running = false;
		__its_unit_test_cpp_load.join();
		std::cout << "\033[0;32m"
				  << "Success"
				  << "\x1b[0m"
				  << " ("
				  << (unsigned int)elapsed_time / 1000 << "ms)" << std::endl;
		fflush(stdout);
		file.close();
		remove((__current_test_name + std::string("_its_test.log")).c_str());
	}
	catch (std::exception const &e)
	{
		std::cerr.rdbuf(__its_unit_test_stream_buffer_cerr);
		std::cerr << "Error : " << e.what() << std::endl;
		exit(EXIT_FAILURE);
	}
}

void
test4(std::string __current_test_name, void (*function)(void), std::string expected_output_file, unsigned long timeout_millis)
{
	try
	{
		__its_unit_test_stream_buffer_cout = std::cout.rdbuf();
		__its_unit_test_stream_buffer_cerr = std::cerr.rdbuf();
		std::cout << "Check " << __current_test_name << " : ";
		fflush(stdout);
		fflush(stderr);
		__its_unit_test_cpp_running = true;
		__its_unit_test_cpp_load = std::thread(__its_unit_test_cpp_loadingEffect);
		std::ofstream file(__current_test_name + std::string("_its_test.log"));
		std::cout.rdbuf(file.rdbuf());
		std::cerr.rdbuf(file.rdbuf());
		auto start = std::chrono::steady_clock::now();
		std::thread launch_func = std::thread([function]() {function(); __its_unit_test_cpp_running = false; });
		__its_unit_test_cpp_timeout((timeout_millis), launch_func);
		auto end = std::chrono::steady_clock::now();
		__its_unit_test_cpp_load.join();
		unsigned long elapsed_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
		std::cout.rdbuf(__its_unit_test_stream_buffer_cout);
		std::cerr.rdbuf(__its_unit_test_stream_buffer_cerr);
		std::cout << "\033[0;32m"
				  << "Success "
				  << "\x1b[0m"
				  << " (" << (unsigned int)elapsed_time / 1000 << "ms)" << std::endl;
		fflush(stdout);
		file.close();
		assert_file(expected_output_file, __current_test_name + std::string("_its_test.log"));
		remove((__current_test_name + std::string("_its_test.log")).c_str());
	}
	catch (std::exception const &e)
	{
		std::cerr.rdbuf(__its_unit_test_stream_buffer_cerr);
		std::cerr << "Error : " << e.what() << std::endl;
		exit(EXIT_FAILURE);
	}
}