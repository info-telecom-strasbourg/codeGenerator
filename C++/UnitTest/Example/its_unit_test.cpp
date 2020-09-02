#include "its_unit_test.h"

using namespace std;

/* A boolean that indicate if the test is running */
static bool __its_unit_test_cpp_running;

/* The thread that display the loading effect */
static thread __its_unit_test_cpp_load;

/* A stream buffer to store the cout stream */
static streambuf *__its_unit_test_stream_buffer_cout;

/* A stream buffer to store the cerr stream */
static streambuf *__its_unit_test_stream_buffer_cerr;

/**
 * @brief used to catch errors in test functions.
 *
 * This function will end the programm if it is called.
 *
 * @param e: the catched exception
 */
static void
catch_err(exception const &e)
{
	cerr.rdbuf(__its_unit_test_stream_buffer_cerr);
	cerr << "Error : " << e.what() << endl;
	exit(EXIT_FAILURE);
}

/**
 * @brief While the test is running, create a waiting animation.
 *
 * This function use 2 global variables:
 * - __its_unit_test_cpp_running to check if the test is running.
 * - __its_unit_test_stream_buffer_cout to print the loading effect.
 */
static void
__its_unit_test_cpp_loadingEffect()
{
    try
    {
        ostream stream_out(__its_unit_test_stream_buffer_cout);
        int ind = 0;
        while (__its_unit_test_cpp_running)
        {
            switch (ind)
            {
                case 0:
                    stream_out << "   \b\b\b";
                    break;
                case 1:
                    stream_out << ".  \b\b\b";
                    break;
                case 2:
                    stream_out << ".. \b\b\b";
                    break;
                case 3:
                    stream_out << "...\b\b\b";
                    break;
            }
            stream_out.flush();
            ind = (ind + 1) % 4;
            usleep(300000);
        }
        stream_out << "   \b\b\b";
        stream_out.flush();
    }
    catch (exception const &e)
    {
        catch_err(e);
    }
}

/**
 * @brief Stop the programm if the test is not finished after a certain time.
 *
 * This function use 2 global variables:
 * - __its_unit_test_cpp_running: to check if the test is still running.
 * - __its_unit_test_cpp_load: to stop the loading effect properly.
 *
 * @param time: the maximum time, in milliseconds
 * @param launch_func: the thread that execute the tested function
 */
static void
__its_unit_test_cpp_timeout(float time, thread &launch_func)
{
    try
    {
        ostream stream_err(__its_unit_test_stream_buffer_cerr);
        auto start_time = chrono::steady_clock::now();
        while (__its_unit_test_cpp_running)
            if (chrono::duration_cast<chrono::milliseconds>
				(chrono::steady_clock::now() - start_time).count() > time)
            {
                __its_unit_test_cpp_running = false;
                stream_err << "\x1b[1;31m" << "Timeout (" << time << " ms)"
                           << "\x1b[0m" << endl;
                stream_err.flush();
                launch_func.detach();
                __its_unit_test_cpp_load.join();
                exit(EXIT_FAILURE);
            }
        launch_func.join();
    }
    catch (exception const &e)
    {
        catch_err(e);
    }
}

void
__its_assert(string expression_text, bool expression)
{
    try
    {
        if (!(expression))
        {
            ostream stream_err(__its_unit_test_stream_buffer_cerr);
            __its_unit_test_cpp_running = false;
            __its_unit_test_cpp_load.join();
            stream_err << "\x1b[1;31m" << "Failed " << "\x1b[0m" << endl
                       << "assertion failed : " << expression_text << endl;
            stream_err.flush();
            exit(EXIT_FAILURE);
        }
    }
    catch (exception const &e)
    {
        catch_err(e);
    }
}

void
__its_assert_files(string first_file, string second_file)
{
    try
    {
        ostream stream_err(__its_unit_test_stream_buffer_cerr);
        bool equals = false;
        ifstream f1(first_file, ifstream::binary | ifstream::ate);
        if (f1.fail())
        {
            __its_unit_test_cpp_running = false;
            __its_unit_test_cpp_load.join();
            stream_err << "\x1b[1;31m" << "Failed " << "\x1b[0m" << endl
                       << "Failure when opening the file " << first_file
					   << "!" << endl;
            stream_err.flush();
            exit(EXIT_FAILURE);
        }
        ifstream f2(second_file, ifstream::binary | ifstream::ate);
        if (f2.fail())
        {
            __its_unit_test_cpp_running = false;
            __its_unit_test_cpp_load.join();
            stream_err << "\x1b[1;31m" << "Failed " << "\x1b[0m" << endl
                       << "Failure when opening the file " << second_file
					   << "!" << endl;
            stream_err.flush();
            exit(EXIT_FAILURE);
        }
        if (f1.tellg() == f2.tellg())
        {
            f1.seekg(0, ifstream::beg);
            f2.seekg(0, ifstream::beg);
            equals = equal(istreambuf_iterator<char>(f1.rdbuf()),
                               istreambuf_iterator<char>(),
                               istreambuf_iterator<char>(f2.rdbuf()));
        }
        f1.close();
        f2.close();
        if (!(equals))
        {
            __its_unit_test_cpp_running = false;
            __its_unit_test_cpp_load.join();
            stream_err << "\x1b[1;31m" << "Failed " << "\x1b[0m" << endl
                       << "The files \"" << first_file << "\" and \""
					   << second_file << "\" are different!" << endl;
            stream_err.flush();
            exit(EXIT_FAILURE);
        }
    }
    catch (exception const &e)
    {
        catch_err(e);
    }
}

/**
 * @brief initialize the environnment for a test.
 *
 * This function use 4 global variables:
 * - __its_unit_test_stream_buffer_cout: to save cout;
 * - __its_unit_test_stream_buffer_cerr: to save cerr;
 * - __its_unit_test_cpp_running: to start the loading effect.
 * - __its_unit_test_cpp_load: the thread for the loading effect.
 *
 * @param __current_test_name: the name of the tested function.
 * @param redir: the name of the file where cout and cerr will be redirected to.
 */
static ofstream
init_test(string __current_test_name, string redir)
{
	__its_unit_test_stream_buffer_cout = cout.rdbuf();
	__its_unit_test_stream_buffer_cerr = cerr.rdbuf();
	cout << "Check " << __current_test_name << " : ";
	fflush(stdout);
	fflush(stderr);
	__its_unit_test_cpp_running = true;
	__its_unit_test_cpp_load = thread(__its_unit_test_cpp_loadingEffect);
	ofstream file(redir);
	cout.rdbuf(file.rdbuf());
	cerr.rdbuf(file.rdbuf());
	return file;
}

/**
 * @brief Reset the environnment (reset cout and cerr).
 *
 * This function use 3 global variables:
 * - __its_unit_test_cpp_running: to stop the loading effect.
 * - __its_unit_test_stream_buffer_cout: to redirect cout to the correct
 * standard output.
 * - __its_unit_test_stream_buffer_cerr: to redirect cerr to the correct
 * standard error output.
 *
 * @param elapsed_time: the time taken by the function to finish.
 * @param file: the file used for redirction.
 */
static void
end_test(unsigned long elapsed_time, ofstream &file)
{
	__its_unit_test_cpp_running = false;
	cout.rdbuf(__its_unit_test_stream_buffer_cout);
	cerr.rdbuf(__its_unit_test_stream_buffer_cerr);
	__its_unit_test_cpp_load.join();
	cout << "\033[0;32m" << "Success" << "\x1b[0m" << " ("
			  << elapsed_time / 1000 << "ms)" << endl;
	fflush(stdout);
	file.close();
}


void
test1(string __current_test_name, void (*function)(void))
{
	try
	{
		ofstream file = init_test(__current_test_name, "/dev/null");
		auto start = chrono::steady_clock::now();
		function();
		auto end = chrono::steady_clock::now();
		unsigned long elapsed_time = chrono::duration_cast<chrono::microseconds>
			(end - start).count();
		end_test(elapsed_time, file);
	}
	catch (exception const &e)
	{
		catch_err(e);
	}
}

void
test2(string __current_test_name, void (*function)(void),
	  unsigned long timeout_millis)
{
	try
	{
		ofstream file = init_test(__current_test_name, "/dev/null");
		auto start = chrono::steady_clock::now();
		thread launch_func = thread([function]()
		{
			function();
			__its_unit_test_cpp_running = false;
		});
		__its_unit_test_cpp_timeout((timeout_millis), launch_func);
		auto end = chrono::steady_clock::now();
		unsigned long elapsed_time = chrono::duration_cast<chrono::microseconds>
			(end - start).count();
		end_test(elapsed_time, file);
	}
	catch (exception const &e)
	{
		catch_err(e);
	}
}

void
test3(string __current_test_name, void (*function)(void),
	  string expected_output_file)
{
	try
	{
		ofstream file = init_test(__current_test_name, __current_test_name +
			string("_its_test.log"));
		auto start = chrono::steady_clock::now();
		function();
		auto end = chrono::steady_clock::now();
		__its_assert_files(expected_output_file, __current_test_name +
			string("_its_test.log"));
		cout.rdbuf(__its_unit_test_stream_buffer_cout);
		cerr.rdbuf(__its_unit_test_stream_buffer_cerr);
		unsigned long elapsed_time = chrono::duration_cast<chrono::microseconds>
			(end - start).count();
		end_test(elapsed_time, file);
		remove((__current_test_name + string("_its_test.log")).c_str());
	}
	catch (exception const &e)
	{
		catch_err(e);
	}
}

void
test4(string __current_test_name, void (*function)(void),
	  string expected_output_file, unsigned long timeout_millis)
{
	try
	{
		ofstream file = init_test(__current_test_name, __current_test_name +
			string("_its_test.log"));
		auto start = chrono::steady_clock::now();
		thread launch_func = thread([function]()
		{
			function();
			__its_unit_test_cpp_running = false;
		});
		__its_unit_test_cpp_timeout((timeout_millis), launch_func);
		auto end = chrono::steady_clock::now();
		unsigned long elapsed_time = chrono::duration_cast<chrono::microseconds>
			(end - start).count();
		end_test(elapsed_time, file);
		assert_file(expected_output_file, __current_test_name +
			string("_its_test.log"));
		remove((__current_test_name + string("_its_test.log")).c_str());
	}
	catch (exception const &e)
	{
		catch_err(e);
	}
}