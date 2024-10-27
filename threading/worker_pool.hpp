#ifndef WORKER_POOL_HPP
#define WORKER_POOL_HPP

#include <functional>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>

class WorkerPool
{
public:
    class IJobs
    {
    public:
        virtual ~IJobs() = default;
        virtual void execute() = 0;
    };

    WorkerPool(std::size_t numWorkers = 1);
    ~WorkerPool();

    void addJob(const std::function<void()> &jobToExecute);
    void addJob(std::shared_ptr<IJobs> job);

private:
    void workerThread();

    std::vector<std::thread> threads_;
    std::queue<std::function<void()>> jobs_;

    std::mutex mutex_;
    std::condition_variable cv_;
    std::atomic<bool> is_active_{true};
};

#endif /* WORKER_POOL_HPP */
