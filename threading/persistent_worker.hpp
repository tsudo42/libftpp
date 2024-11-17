#ifndef PERSISTENT_WORKER_HPP
#define PERSISTENT_WORKER_HPP

#include <thread>
#include <functional>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <deque>
#include <utility>
#include <stdexcept>

class PersistentWorker
{
public:
    class DuplicateJobName;
    class JobNotFound;
    class InvalidWorker;

    PersistentWorker() = default;

    PersistentWorker(const PersistentWorker &) = delete;
    PersistentWorker &operator=(const PersistentWorker &) = delete;

    PersistentWorker(PersistentWorker &&other) noexcept;
    PersistentWorker &operator=(PersistentWorker &&other) noexcept;

    ~PersistentWorker();

    void addTask(const std::string &name, const std::function<void()> &jobToExecute);
    void removeTask(const std::string &name);

    void setExceptionHandler(const std::function<void(const std::exception &)> &handler);

    void setSleepDuration(std::chrono::milliseconds duration);

    bool isValid() const;

private:
    std::deque<std::pair<std::string, std::function<void()>>>::iterator findJob_(const std::string &name);
    void run_();

    std::atomic<bool> valid_{true};

    std::function<void(const std::exception &)> exception_handler_;
    std::chrono::milliseconds sleep_duration_{50};

    std::mutex mutex_;
    std::condition_variable cv_;
    std::deque<std::pair<std::string, std::function<void()>>> jobs_;

    std::thread thread_{&PersistentWorker::run_, this};
};

class PersistentWorker::DuplicateJobName : public std::runtime_error
{
public:
    virtual ~DuplicateJobName() = default;
    DuplicateJobName();
};

class PersistentWorker::JobNotFound : public std::runtime_error
{
public:
    virtual ~JobNotFound() = default;
    JobNotFound();
};

class PersistentWorker::InvalidWorker : public std::runtime_error
{
public:
    virtual ~InvalidWorker() = default;
    InvalidWorker();
};

#endif /* PERSISTENT_WORKER_HPP */
