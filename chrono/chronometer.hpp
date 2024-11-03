#ifndef CHRONOMETER_HPP
#define CHRONOMETER_HPP

#include <chrono>

class Chronometer
{
public:
    void start();
    void pause();
    void reset();

    std::chrono::milliseconds elapsed() const;

private:
    bool active_{false};
    std::chrono::time_point<std::chrono::system_clock> start_time_;
    std::chrono::milliseconds total_time_{0};
};

#endif /* CHRONOMETER_HPP */
