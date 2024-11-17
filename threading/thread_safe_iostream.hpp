#ifndef THREAD_SAFE_IOSTREAM_HPP
#define THREAD_SAFE_IOSTREAM_HPP

#include <thread>
#include <iostream>
#include <sstream>
#include <string>
#include <mutex>

class ThreadSafeIOStream
{
public:
    ThreadSafeIOStream() = default;

    void setPrefix(const std::string &prefix);

    template <typename T>
    ThreadSafeIOStream &operator<<(const T &data)
    {
        buffer_ << data;
        return *this;
    }

    ThreadSafeIOStream &operator<<(std::ostream &(*manip)(std::ostream &));

private:
    std::mutex mutex_;
    static thread_local std::string prefix_;
    static thread_local std::ostringstream buffer_;
};

extern ThreadSafeIOStream threadSafeCout;

#endif /* THREAD_SAFE_IOSTREAM_HPP */
