#include "chrono/timer.hpp"

void Timer::start(std::chrono::milliseconds duration)
{
    timeout = std::chrono::system_clock::now() + duration;
    active = true;
}

bool Timer::isTimeout() const
{
    return active && std::chrono::system_clock::now() >= timeout;
}

void Timer::reset()
{
    active = false;
}
