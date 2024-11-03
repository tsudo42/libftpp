#include "design_patterns/observable_value.hpp"
#include <cassert>
#include <iostream>

void test_set_and_get()
{
    ObservableValue<int> observable;
    observable.set(10);
    assert(observable.get() == 10);
    std::cout << "Test1: set and get - Passed" << std::endl;
}

void test_subscription_and_notification()
{
    ObservableValue<int> observable;
    bool notified = false;
    observable.subscribe([&notified](const int &value)
                         {
        notified = true;
        assert(value == 20); });
    observable.set(20);
    assert(notified);
    std::cout << "Test2: subscription and notification - Passed" << std::endl;
}

void test_multiple_subscriptions()
{
    ObservableValue<int> observable;
    int notify_count = 0;
    observable.subscribe([&notify_count](const int &value)
                         {
        notify_count++;
        assert(value == 30); });
    observable.subscribe([&notify_count](const int &value)
                         {
        notify_count++;
        assert(value == 30); });
    observable.set(30);
    assert(notify_count == 2);
    std::cout << "Test3: multiple subscriptions - Passed" << std::endl;
}

int main()
{
    test_set_and_get();
    test_subscription_and_notification();
    test_multiple_subscriptions();

    std::cout << "Finished all tests." << std::endl;
    return 0;
}
