#include "chrono/timer.hpp"
#include <cassert>
#include <iostream>
#include <thread>

void test_timer_start_sets_timeout()
{
    Timer timer;
    auto duration = std::chrono::milliseconds(100);
    auto expected_timeout = std::chrono::system_clock::now() + duration;
    timer.start(duration);
    assert(timer.isTimeout() == false);
    while (true)
    {
        bool is_timeout = timer.isTimeout();
        if (std::chrono::system_clock::now() >= expected_timeout)
        {
            break;
        }
        assert(is_timeout == false);
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    assert(timer.isTimeout() == true);

    std::cout << "Test1: Timer start sets timeout - Passed" << std::endl;
}

void test_timer_start_sets_active()
{
    Timer timer;
    timer.start(std::chrono::milliseconds(100));
    assert(timer.isTimeout() == false);

    std::cout << "Test2: Timer start sets active - Passed" << std::endl;
}

void test_timer_isTimeout_immediately_after_start()
{
    Timer timer;
    timer.start(std::chrono::milliseconds(100));
    assert(timer.isTimeout() == false);

    std::cout << "Test3: Timer isTimeout immediately after start - Passed" << std::endl;
}

void test_timer_isTimeout_after_duration()
{
    Timer timer;
    timer.start(std::chrono::milliseconds(100));
    std::this_thread::sleep_for(std::chrono::milliseconds(150));
    assert(timer.isTimeout() == true);

    std::cout << "Test4: Timer isTimeout after duration - Passed" << std::endl;
}

int main()
{
    test_timer_start_sets_timeout();
    test_timer_start_sets_active();
    test_timer_isTimeout_immediately_after_start();
    test_timer_isTimeout_after_duration();

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
