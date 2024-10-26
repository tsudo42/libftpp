#include "threading/persistent_worker.hpp"

PersistentWorker::PersistentWorker(PersistentWorker &&other) noexcept
    : valid_(other.valid_.load()), exception_handler_(std::move(other.exception_handler_)), sleep_duration_(other.sleep_duration_), jobs_(std::move(other.jobs_))
{
    other.valid_.store(false);
    if (other.thread_.joinable())
    {
        other.cv_.notify_all();
        other.thread_.join();
    }

    thread_ = std::thread(&PersistentWorker::run_, this);
}

PersistentWorker &PersistentWorker::operator=(PersistentWorker &&other) noexcept
{
    if (this != &other)
    {
        valid_.store(false);
        if (thread_.joinable())
        {
            cv_.notify_all();
            thread_.join();
        }

        valid_.store(other.valid_.load());
        sleep_duration_ = other.sleep_duration_;
        exception_handler_ = std::move(other.exception_handler_);
        jobs_ = std::move(other.jobs_);

        thread_ = std::thread(&PersistentWorker::run_, this);

        other.valid_.store(false);
        if (other.thread_.joinable())
        {
            other.cv_.notify_all();
            other.thread_.join();
        }
    }
    return *this;
}

PersistentWorker::~PersistentWorker()
{
    valid_.store(false);
    cv_.notify_all();
    if (thread_.joinable())
    {
        thread_.join();
    }
}

void PersistentWorker::addTask(const std::string &name, const std::function<void()> &jobToExecute)
{
    if (!valid_.load())
        throw InvalidWorker();

    std::lock_guard<std::mutex> lock(mutex_);
    if (auto it = findJob_(name); it != jobs_.end())
    {
        throw DuplicateJobName();
    }
    jobs_.push_back({name, jobToExecute});
    cv_.notify_one();
}

void PersistentWorker::removeTask(const std::string &name)
{
    if (!valid_.load())
        throw InvalidWorker();

    std::lock_guard<std::mutex> lock(mutex_);
    auto it = findJob_(name);
    if (it == jobs_.end())
    {
        throw JobNotFound();
    }
    jobs_.erase(it);
}

void PersistentWorker::setExceptionHandler(const std::function<void(const std::exception &)> &handler)
{
    std::lock_guard<std::mutex> lock(mutex_);
    exception_handler_ = handler;
}

void PersistentWorker::setSleepDuration(std::chrono::milliseconds duration)
{
    std::lock_guard<std::mutex> lock(mutex_);
    sleep_duration_ = duration;
}

bool PersistentWorker::isValid() const
{
    return valid_.load();
}

std::deque<std::pair<std::string, std::function<void()>>>::iterator PersistentWorker::findJob_(const std::string &name)
{
    /* mutex_ should be locked */
    for (auto it = jobs_.begin(); it != jobs_.end(); ++it)
    {
        if (it->first == name)
        {
            return it;
        }
    }

    return jobs_.end();
}

void PersistentWorker::run_()
{
    std::unique_lock<std::mutex> lock(mutex_);
    while (true)
    {
        cv_.wait(lock, [this]
                 { return !jobs_.empty() || !valid_.load(); });

        if (!valid_.load())
            return;

        if (jobs_.empty())
        {
            lock.unlock();
        }
        else
        {
            auto job_pair = jobs_.front();
            jobs_.pop_front();
            jobs_.push_back(job_pair);
            auto job = job_pair.second;

            lock.unlock();

            try
            {
                job();
            }
            catch (const std::exception &e)
            {
                if (exception_handler_)
                    exception_handler_(e);
            }
        }

        std::this_thread::sleep_for(sleep_duration_);

        lock.lock();
    }
}

PersistentWorker::DuplicateJobName::DuplicateJobName()
    : std::runtime_error("Job name already registered") {}

PersistentWorker::JobNotFound::JobNotFound()
    : std::runtime_error("Job name not found") {}

PersistentWorker::InvalidWorker::InvalidWorker()
    : std::runtime_error("Worker is no longer valid") {}
