#include "its_unit_test.h"

using namespace std;

/* A boolean that indicate if the test is running */
static bool test_running;

/* The thread that display the loading effect */
static thread load_effect;

/* A stream buffer to store the cout stream */
static streambuf *saved_cout;

/* A stream buffer to store the cerr stream */
static streambuf *saved_cerr;

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
	cerr.rdbuf(saved_cerr);
	cerr << "Error : " << e.what() << endl;
	exit(EXIT_FAILURE);
}

/**
 * @brief While the test is running, create a waiting animation.
 *
 * This function use 2 global variables:
 * - test_running to check if the test is running.
 * - saved_cout to print the loading effect.
 */
static void
load_effectingEffect()
{
    try
    {
        ostream stream_out(saved_cout);
        int ind = 0;
        while (test_running)
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
 * - test_running: to check if the test is still running.
 * - load_effect: to stop the loading effect properly.
 *
 * @param time: the maximum time, in milliseconds
 * @param launch_func: the thread that execute the tested function
 */
static void
__its_unit_test_cpp_timeout(float time, thread &launch_func)
{
    try
    {
        ostream stream_err(saved_cerr);
        auto start_time = chrono::steady_clock::now();
        while (test_running)
            if (chrono::duration_cast<chrono::milliseconds>
				(chrono::steady_clock::now() - start_time).count() > time)
            {
                test_running = false;
                stream_err << "\x1b[1;31m" << "Timeout (" << time << " ms)"
                           << "\x1b[0m" << endl;
                stream_err.flush();
                launch_func.detach();
                load_effect.join();
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
__assert_unittest_its(string expression_text, bool expression)
{
    try
    {
        if (!(expression))
        {
            ostream stream_err(saved_cerr);
            test_running = false;
            load_effect.join();
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
___assert_file_unittest_its(string first_file, string second_file)
{
    try
    {
        ostream stream_err(saved_cerr);
        bool equals = false;
        ifstream f1(first_file, ifstream::binary | ifstream::ate);
        if (f1.fail())
        {
            test_running = false;
            load_effect.join();
            stream_err << "\x1b[1;31m" << "Failed " << "\x1b[0m" << endl
                       << "Failure when opening the file " << first_file
					   << "!" << endl;
            stream_err.flush();
            exit(EXIT_FAILURE);
        }
        ifstream f2(second_file, ifstream::binary | ifstream::ate);
        if (f2.fail())
        {
            test_running = false;
            load_effect.join();
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
            test_running = false;
            load_effect.join();
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
 * - saved_cout: to save cout;
 * - saved_cerr: to save cerr;
 * - test_running: to start the loading effect.
 * - load_effect: the thread for the loading effect.
 *
 * @param __current_test_name: the name of the tested function.
 * @param redir: the name of the file where cout and cerr will be redirected to.
 * @param running_effect: indicate if the running effect should be launched.
 * @return an ofstream to redirect files.
 */
static ofstream
init_test(string __current_test_name, string redir, bool running_effect)
{
	saved_cout = cout.rdbuf();
	saved_cerr = cerr.rdbuf();
	cout << "Check " << __current_test_name << " : ";
	fflush(stdout);
	fflush(stderr);
	test_running = true;
	if(running_effect)
		load_effect = thread(load_effectingEffect);
	ofstream file(redir);
	cout.rdbuf(file.rdbuf());
	cerr.rdbuf(file.rdbuf());
	return file;
}

/**
 * @brief Reset the environnment (reset cout and cerr).
 *
 * This function use 3 global variables:
 * - test_running: to stop the loading effect.
 * - saved_cout: to redirect cout to the correct
 * standard output.
 * - saved_cerr: to redirect cerr to the correct
 * standard error output.
 *
 * @param elapsed_time: the time taken by the function to finish.
 * @param file: the file used for redirction.
 */
static void
end_test(unsigned long elapsed_time, ofstream &file)
{
	test_running = false;
	cout.rdbuf(saved_cout);
	cerr.rdbuf(saved_cerr);
	load_effect.join();
	cout << "\033[0;32m" << "Success" << "\x1b[0m" << " ("
			  << elapsed_time / 1000 << "ms)" << endl;
	fflush(stdout);
	file.close();
}


void
__test_classic_unittest_its(string __current_test_name, void (*function)(void))
{
	try
	{
		ofstream file = init_test(__current_test_name, "/dev/null", true);
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
__test_timeout_unittest_its(string __current_test_name, void (*function)(void),
	  unsigned long timeout_millis)
{
	try
	{
		ofstream file = init_test(__current_test_name, "/dev/null", true);
		auto start = chrono::steady_clock::now();
		thread launch_func = thread([function]()
		{
			function();
			test_running = false;
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
__test_output_unittest_its(string __current_test_name, void (*function)(void),
	  string expected_output_file)
{
	try
	{
		ofstream file = init_test(__current_test_name, __current_test_name +
			string("_its_test.log"), true);
		auto start = chrono::steady_clock::now();
		function();
		auto end = chrono::steady_clock::now();
		___assert_file_unittest_its(expected_output_file, __current_test_name +
			string("_its_test.log"));
		cout.rdbuf(saved_cout);
		cerr.rdbuf(saved_cerr);
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
__test_output_timeout_unittest_its(string __current_test_name,
	void (*function)(void), string expected_output_file,
	unsigned long timeout_millis)
{
	try
	{
		ofstream file = init_test(__current_test_name, __current_test_name +
			string("_its_test.log"), true);
		auto start = chrono::steady_clock::now();
		thread launch_func = thread([function]()
		{
			function();
			test_running = false;
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

void
__exit_test_unittest(string __current_test_name, void (*function)(void), int exit_code)
{
	try
	{
		pid_t cpid;
		int status;
		ofstream file = init_test(__current_test_name, "/dev/null", false);
		ostream stream_err(saved_cerr);
		auto start = chrono::steady_clock::now();
		switch (cpid = fork()) {
			case -1:
				test_running = false;
				load_effect.join();
				stream_err << "fork failed" << endl;
				exit(EXIT_FAILURE);
			case 0:
				function();
				stream_err << "\x1b[1;31m" << "Your function didn't " <<
					"called exit !" << "\x1b[0m" << endl;
				kill(getppid(), SIGKILL);
				exit(EXIT_FAILURE);
			default:
				load_effect = thread(load_effectingEffect);
				break;
		}
		waitpid(cpid, &status, 0);
		if(WIFEXITED(status))
		{
			if(WEXITSTATUS(status) != exit_code)
			{
				test_running = false;
				load_effect.join();
				stream_err << "\x1b[1;31m" << "Failed\nassertion failed : "
					<< WEXITSTATUS(status) << " != " << exit_code << "\x1b[0m"
					<< endl;
				exit(EXIT_FAILURE);
			}
		}
		else
			__assert_unittest_its("Not finished with exit", false);
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
