#include "threading/thread.hpp"

Thread::Thread(const std::string &name, std::function<void()> funcToExecute)
    : name_(name), funcToExecute_(funcToExecute), thread_(nullptr) {}

Thread::~Thread() { stop(); }

void Thread::start()
{
    if (isRunning())
    {
        throw ThreadAlreadyRunningException();
    }

    thread_ = std::make_unique<std::thread>([this]()
                                            {
        threadSafeCout.setPrefix(name_);
        funcToExecute_(); });
}

void Thread::stop()
{
    if (isRunning())
    {
        thread_->join();
    }
    thread_.reset();
}

bool Thread::isRunning() const
{
    return thread_ && thread_->joinable();
}

Thread::Id Thread::getId() const
{
    return thread_->get_id();
}

Thread::ThreadAlreadyRunningException::ThreadAlreadyRunningException()
    : std::runtime_error("Thread is already running") {}
