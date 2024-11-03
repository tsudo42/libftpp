#ifndef TIMER_HPP
#define TIMER_HPP

#include <chrono>

class Timer
{
public:
    void start(std::chrono::milliseconds duration);
    bool isTimeout() const;
    void reset();

private:
    bool active{false};
    std::chrono::time_point<std::chrono::system_clock> timeout;
};

#endif /* TIMER_HPP */
