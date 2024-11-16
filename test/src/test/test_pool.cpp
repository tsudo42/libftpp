#include "libftpp.hpp"

#include <iostream>
#include <cassert>

class TestClass
{
public:
    TestClass()
    {
        std::cout << "TestClass constructed" << std::endl;
        ++constructedCount;
    }

    TestClass(int x) : x_(x)
    {
        std::cout << "TestClass constructed with x = " << x_ << std::endl;
        ++constructedCount;
    }

    TestClass(const TestClass &other) : x_(other.x_)
    {
        std::cout << "TestClass copy-constructed with x = " << x_ << std::endl;
        ++constructedCount;
    }

    TestClass &operator=(const TestClass &other)
    {
        x_ = other.x_;
        std::cout << "TestClass copy-assigned with x = " << x_ << std::endl;
        return *this;
    }

    TestClass(TestClass &&other) : x_(other.x_)
    {
        std::cout << "TestClass move-constructed with x = " << x_ << std::endl;
        ++constructedCount;
    }

    TestClass &operator=(TestClass &&other)
    {
        x_ = other.x_;
        std::cout << "TestClass move-assigned with x = " << x_ << std::endl;
        return *this;
    }

    ~TestClass()
    {
        std::cout << "TestClass destructed\n";
        ++destructedCount;
    }

    void setValue(int x) { x_ = x; }
    int getValue() const { return x_; }

    static int getConstructedCount() { return constructedCount; }
    static int getDestructedCount() { return destructedCount; }

    static void resetCounters()
    {
        constructedCount = 0;
        destructedCount = 0;
    }

private:
    int x_;
    static int constructedCount;
    static int destructedCount;
};

int TestClass::constructedCount = 0;
int TestClass::destructedCount = 0;

void testObjectCreationAndDestruction()
{
    std::cout << "\nRunning testObjectCreationAndDestruction...\n";
    TestClass::resetCounters();

    {
        Pool<TestClass> pool(2);
        auto obj1 = pool.acquire(10);
        assert(obj1->getValue() == 10);
        assert(TestClass::getConstructedCount() == 1);

        auto obj2 = pool.acquire(20);
        assert(obj2->getValue() == 20);
        assert(TestClass::getConstructedCount() == 2);
    }

    assert(TestClass::getConstructedCount() == 2);
    assert(TestClass::getDestructedCount() == 2);

    std::cout << "\ntestObjectCreationAndDestruction passed.\n";
}

void testMemoryReuseWithoutAllocation()
{
    std::cout << "\nRunning testMemoryReuseWithoutAllocation...\n";
    TestClass::resetCounters();

    Pool<TestClass> pool(2);

    {
        auto obj1 = pool.acquire(10);
        auto obj2 = pool.acquire(20);
        assert(TestClass::getConstructedCount() == 2);
        assert(TestClass::getDestructedCount() == 0);
        (void)obj1;
        (void)obj2;
    }
    {
        auto obj3 = pool.acquire(30);
        auto obj4 = pool.acquire(40);
        assert(TestClass::getConstructedCount() == 4);
        assert(TestClass::getDestructedCount() == 2);
        (void)obj3;
        (void)obj4;
    }

    assert(TestClass::getConstructedCount() == 4);
    assert(TestClass::getDestructedCount() == 4);

    std::cout << "\ntestMemoryReuseWithoutAllocation passed.\n";
}

void testPoolDestruction()
{
    std::cout << "\nRunning testPoolDestruction...\n";
    TestClass::resetCounters();

    {
        Pool<TestClass> pool(3);
        auto obj1 = pool.acquire(10);
        auto obj2 = pool.acquire(20);
        auto obj3 = pool.acquire(30);

        assert(TestClass::getConstructedCount() == 3);
        assert(TestClass::getDestructedCount() == 0);
        (void)obj1;
        (void)obj2;
        (void)obj3;
    }

    assert(TestClass::getConstructedCount() == 3);
    assert(TestClass::getDestructedCount() == 3);

    std::cout << "\ntestPoolDestruction passed.\n";
}

void testMemoryReuseByAddressComparison()
{
    std::cout << "\nRunning testMemoryReuseByAddressComparison...\n";
    TestClass::resetCounters();

    Pool<TestClass> pool(1);
    void *address1, *address2;

    {
        auto obj1 = pool.acquire(10);
        address1 = obj1.operator->();
        assert(obj1->getValue() == 10);
    }
    {
        auto obj2 = pool.acquire(20);
        address2 = obj2.operator->();
        assert(obj2->getValue() == 20);
    }
    assert(address1 == address2);

    std::cout << "\ntestMemoryReuseByAddressComparison passed.\n";
}

int main()
{
    testObjectCreationAndDestruction();
    testMemoryReuseWithoutAllocation();
    testPoolDestruction();
    testMemoryReuseByAddressComparison();

    std::cout << "\nAll tests passed.\n";
    return 0;
}
