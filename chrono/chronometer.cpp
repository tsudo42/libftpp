#include "chrono/chronometer.hpp"

void Chronometer::start()
{
    active_ = true;
    start_time_ = std::chrono::system_clock::now();
}

void Chronometer::pause()
{
    active_ = false;
    total_time_ += std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start_time_);
}

void Chronometer::reset()
{
    active_ = false;
    total_time_ = std::chrono::milliseconds{0};
}

std::chrono::milliseconds Chronometer::elapsed() const
{
    if (active_)
    {
        return total_time_ + std::chrono::duration_cast<std::chrono::milliseconds>(
                                 std::chrono::system_clock::now() - start_time_);
    }
    return total_time_;
}
