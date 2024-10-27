#include "threading/worker_pool.hpp"

WorkerPool::WorkerPool(std::size_t numWorkers)
    : is_active_(true)
{
    for (std::size_t i = 0; i < numWorkers; ++i)
    {
        threads_.emplace_back(&WorkerPool::workerThread, this);
    }
}

WorkerPool::~WorkerPool()
{
    is_active_ = false;
    cv_.notify_all();

    for (auto &worker : threads_)
    {
        if (worker.joinable())
        {
            worker.join();
        }
    }
}

void WorkerPool::addJob(const std::function<void()> &jobToExecute)
{
    {
        std::lock_guard<std::mutex> lock(mutex_);
        jobs_.emplace(jobToExecute);
    }
    cv_.notify_one();
}

void WorkerPool::addJob(std::shared_ptr<IJobs> job)
{
    addJob([job]()
           { job->execute(); });
}

void WorkerPool::workerThread()
{
    while (true)
    {
        std::function<void()> job;
        {
            std::unique_lock<std::mutex> lock(mutex_);
            cv_.wait(lock, [this]()
                     { return !is_active_.load() || !jobs_.empty(); });

            if (!is_active_.load())
                return;

            job = std::move(jobs_.front());
            jobs_.pop();
        }

        try
        {
            if (job)
                job();
        }
        catch (...)
        {
        }
    }
}