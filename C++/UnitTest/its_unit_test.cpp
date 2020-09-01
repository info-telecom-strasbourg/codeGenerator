#include "its_unit_test.h"

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
init_test()
{

}

void
test1()
{

}

void
__its_assert(char *expression_text, bool expression)
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
                       << "assertion failed : " << #expression_text << std::endl;
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
__its_assert_files(char *first_file, char* second_file)
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
                       << "The files \"" << first_file "\" and \"" << second_file << "\" are different!" << std::endl;
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