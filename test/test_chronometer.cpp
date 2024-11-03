#include "chrono/chronometer.hpp"
#include <cassert>
#include <iostream>
#include <thread>

void test_elapsed_when_active()
{
    Chronometer chronometer;
    chronometer.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    auto elapsed_time = chronometer.elapsed();
    assert(elapsed_time.count() >= 100);
    std::cout << "Test1: elapsed when active - Passed" << std::endl;
}

void test_elapsed_when_paused()
{
    Chronometer chronometer;
    chronometer.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    chronometer.pause();
    auto elapsed_time = chronometer.elapsed();
    assert(elapsed_time.count() >= 100);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    assert(chronometer.elapsed() == elapsed_time);
    std::cout << "Test2: elapsed when paused - Passed" << std::endl;
}

void test_elapsed_after_reset()
{
    Chronometer chronometer;
    chronometer.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    chronometer.reset();
    assert(chronometer.elapsed().count() == 0);
    std::cout << "Test3: elapsed after reset - Passed" << std::endl;
}

int main()
{
    test_elapsed_when_active();
    test_elapsed_when_paused();
    test_elapsed_after_reset();

    std::cout << "Finished all tests." << std::endl;
    return 0;
}
