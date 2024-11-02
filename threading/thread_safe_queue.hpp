#ifndef THREAD_SAFE_QUEUE_HPP
#define THREAD_SAFE_QUEUE_HPP

#include <deque>
#include <thread>
#include <mutex>
#include <stdexcept>

template <typename TType>
class ThreadSafeQueue
{
public:
    class EmptyQueueException : public std::runtime_error
    {
    public:
        virtual ~EmptyQueueException() = default;
        EmptyQueueException() : std::runtime_error("Queue is empty") {}
    };

    ThreadSafeQueue() = default;

    void push_back(const TType &newElement)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.push_back(newElement);
    }
    void push_back(TType &&newElement)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.push_back(std::move(newElement));
    }

    void push_front(const TType &newElement)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.push_front(newElement);
    }
    void push_front(TType &&newElement)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.push_front(std::move(newElement));
    }

    TType pop_back()
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (queue_.empty())
        {
            throw EmptyQueueException();
        }
        TType element = std::move(queue_.back());
        queue_.pop_back();
        return element;
    }

    TType pop_front()
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (queue_.empty())
        {
            throw EmptyQueueException();
        }
        TType element = std::move(queue_.front());
        queue_.pop_front();
        return element;
    }

    std::unique_ptr<TType> pop_back_ptr()
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (queue_.empty())
        {
            return nullptr;
        }
        std::unique_ptr<TType> element = std::make_unique<TType>(std::move(queue_.back()));
        queue_.pop_back();
        return element;
    }

    std::unique_ptr<TType> pop_front_ptr()
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (queue_.empty())
        {
            return nullptr;
        }
        std::unique_ptr<TType> element = std::make_unique<TType>(std::move(queue_.front()));
        queue_.pop_front();
        return element;
    }

    void clear()
    {
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.clear();
    }

    size_t size()
    {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.size();
    }

    bool empty()
    {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.empty();
    }

    TType &front()
    {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.front();
    }

    TType &back()
    {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.back();
    }

private:
    std::deque<TType> queue_;
    std::mutex mutex_;
};

#endif /* THREAD_SAFE_QUEUE_HPP */
