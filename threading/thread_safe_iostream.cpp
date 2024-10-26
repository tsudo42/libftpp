#include "threading/thread_safe_iostream.hpp"

thread_local std::string ThreadSafeIOStream::prefix_;
thread_local std::ostringstream ThreadSafeIOStream::buffer_;

void ThreadSafeIOStream::setPrefix(const std::string &prefix)
{
    prefix_ = prefix;
}

ThreadSafeIOStream &ThreadSafeIOStream::operator<<(std::ostream &(*manip)(std::ostream &))
{
    if (manip == static_cast<std::ostream &(*)(std::ostream &)>(std::endl))
    {
        std::lock_guard<std::mutex> lock(mutex_);
        std::cout << prefix_ << buffer_.str() << std::endl;
        buffer_.str("");
        buffer_.clear();
    }
    else
    {
        buffer_ << manip;
    }
    return *this;
}

ThreadSafeIOStream threadSafeCout;
