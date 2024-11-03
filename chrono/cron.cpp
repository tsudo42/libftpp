#include "chrono/cron.hpp"

Cron::Cron(std::function<void()> task, std::chrono::seconds interval)
    : task_(task), interval_(interval), running_(false) {}

Cron::~Cron() { stop(); }

Cron::Cron(Cron &&other) noexcept
    : task_(std::move(other.task_)),
      interval_(other.interval_),
      running_(other.running_.load()),
      scheduler_thread_(std::move(other.scheduler_thread_)),
      next_run_(other.next_run_)
{
    other.running_.store(false);
}

Cron &Cron::operator=(Cron &&other) noexcept
{
    if (this != &other)
    {
        stop();

        task_ = std::move(other.task_);
        interval_ = other.interval_;
        running_.store(other.running_.load());
        scheduler_thread_ = std::move(other.scheduler_thread_);
        next_run_ = other.next_run_;

        other.running_.store(false);
    }
    return *this;
}

void Cron::start()
{
    if (running_.load())
        return;

    next_run_ = std::chrono::system_clock::now() + interval_;

    running_.store(true);
    scheduler_thread_ = std::thread(&Cron::subroutine, this);
}

void Cron::stop()
{
    running_.store(false);
    if (scheduler_thread_.joinable())
    {
        scheduler_thread_.join();
    }
}

void Cron::run_once()
{
    if (task_)
    {
        task_();
    }
}

bool Cron::is_running() const
{
    return running_.load();
}

void Cron::subroutine()
{
    try
    {
        while (running_.load())
        {
            auto now = std::chrono::system_clock::now();
            if (now >= next_run_)
            {
                next_run_ = std::chrono::system_clock::now() + interval_;
                if (task_)
                {
                    task_();
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
    catch (const std::exception &e)
    {
        running_.store(false);
    }
}
