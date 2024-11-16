#include <iostream>
#include <chrono>
#include <thread>
#include <atomic>
#include <cassert>
#include "chrono/cron.hpp"

void test_run_once()
{
    std::atomic<int> counter{0};
    Cron cron([&counter]()
              { counter.store(counter.load() + 1); }, std::chrono::seconds(5));

    cron.run_once();
    assert(counter.load() == 1);
    std::cout << "test_run_once passed!" << std::endl;
}

void test_start_stop()
{
    std::atomic<int> counter{0};
    Cron cron([&counter]()
              { counter.store(counter.load() + 1); }, std::chrono::seconds(1));

    cron.start();
    std::this_thread::sleep_for(std::chrono::seconds(3));
    cron.stop();

    // 実行間隔が1秒なので、約3秒で2回以上実行されることを期待します
    assert(counter.load() >= 2);
    std::cout << "test_start_stop passed!" << std::endl;
}

void test_move_constructor()
{
    std::atomic<int> counter{0};
    Cron cron1([&counter]()
               { counter.store(counter.load() + 1); }, std::chrono::seconds(1));

    Cron cron2 = std::move(cron1);
    cron2.run_once();
    assert(counter.load() == 1);

    std::cout << "test_move_constructor passed!" << std::endl;
}

void test_move_assignment()
{
    std::atomic<int> counter{0};
    Cron cron1([&counter]()
               { counter.store(counter.load() + 1); }, std::chrono::seconds(1));
    Cron cron2([&counter]()
               { counter.store(counter.load() + 2); }, std::chrono::seconds(1));

    cron2 = std::move(cron1);
    cron2.run_once();
    assert(counter.load() == 1);

    std::cout << "test_move_assignment passed!" << std::endl;
}

void test_multiple_cron()
{
    std::atomic<int> counter{0};

    Cron cron1([&counter]()
               { counter.store(counter.load() + 2); }, std::chrono::seconds(3));
    Cron cron2([&counter]()
               { counter.store(counter.load() + 7); }, std::chrono::seconds(5));

    cron1.start();
    cron2.start();

    std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
    std::chrono::system_clock::time_point end = std::chrono::system_clock::now() + std::chrono::seconds(16);

    std::cout << "counter : " << counter.load() << std::flush;
    while (std::chrono::system_clock::now() < end)
    {
        std::cout << "\rcounter : " << counter.load() << " " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start).count() << "ms" << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    assert(counter.load() == 31);
    assert(cron1.is_running());
    assert(cron2.is_running());

    cron1.stop();
    cron2.stop();

    end = std::chrono::system_clock::now() + std::chrono::seconds(7);

    std::cout << "\rstopped.                  " << std::endl;
    std::cout << "counter : " << counter.load() << std::flush;
    while (std::chrono::system_clock::now() < end)
    {
        std::cout << "\rcounter : " << counter.load() << " " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start).count() << "ms" << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    std::cout << std::endl;

    assert(counter.load() == 31);
    assert(!cron1.is_running());
    assert(!cron2.is_running());

    std::cout << "test_multiple_cron passed!" << std::endl;
}

int main()
{
    test_run_once();
    test_start_stop();
    test_move_constructor();
    test_move_assignment();
    test_multiple_cron();

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
