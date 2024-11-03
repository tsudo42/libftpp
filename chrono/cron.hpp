#ifndef CRON_HPP
#define CRON_HPP

#include <chrono>
#include <functional>
#include <thread>
#include <atomic>

class Cron
{
public:
    Cron(std::function<void()> task, std::chrono::seconds interval);
    ~Cron();

    Cron(const Cron &) = delete;
    Cron &operator=(const Cron &) = delete;

    Cron(Cron &&other) noexcept;
    Cron &operator=(Cron &&other) noexcept;

    void start();
    void stop();
    void run_once();

    bool is_running() const;

private:
    void subroutine();

    std::function<void()> task_;
    std::chrono::seconds interval_;

    std::atomic<bool> running_;
    std::thread scheduler_thread_;
    std::chrono::system_clock::time_point next_run_;
};

#endif /* CRON_HPP */
