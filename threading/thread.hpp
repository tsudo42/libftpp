#ifndef THREAD_HPP
#define THREAD_HPP

#include <thread>
#include <functional>
#include <string>
#include <stdexcept>
#include "threading/thread_safe_iostream.hpp"

class Thread
{
public:
    typedef std::thread::id Id;

    class ThreadAlreadyRunningException;

    Thread(const std::string &name, std::function<void()> funcToExecute);
    ~Thread();
    void start();
    void stop();

    Thread(const Thread &) = delete;
    Thread &operator=(const Thread &) = delete;

    Thread(Thread &&) noexcept = default;
    Thread &operator=(Thread &&) noexcept = default;

    bool isRunning() const;
    Id getId() const;

private:
    std::string name_;
    std::function<void()> funcToExecute_;
    std::unique_ptr<std::thread> thread_;
};

class Thread::ThreadAlreadyRunningException : public std::runtime_error
{
public:
    virtual ~ThreadAlreadyRunningException() = default;
    ThreadAlreadyRunningException();
};

#endif /* THREAD_HPP */
