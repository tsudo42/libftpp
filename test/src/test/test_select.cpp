#include "misc/select.hpp"
#include <iostream>
#include <type_traits>

void test_select()
{
    static_assert(std::is_same<select<0, int, double, char>::type, int>::value, "Test1 failed");
    static_assert(std::is_same<select<1, int, double, char>::type, double>::value, "Test2 failed");
    static_assert(std::is_same<select<2, int, double, char>::type, char>::value, "Test3 failed");

    std::cout << "Test: default select - Passed" << std::endl;
}

void test_alias()
{
    static_assert(std::is_same<Select<0, int, double, char>, int>::value, "Test1 failed");
    static_assert(std::is_same<Select<1, int, double, char>, double>::value, "Test2 failed");
    static_assert(std::is_same<Select<2, int, double, char>, char>::value, "Test3 failed");

    std::cout << "Test: Select alias - Passed" << std::endl;
}

void test_fail()
{
    // static_assert(std::is_same<Select<4, int, double, char>, double>::value, "Test1 failed");
}

int main()
{
    test_select();
    test_alias();
    test_fail();

    std::cout << "Finished all tests." << std::endl;
    return 0;
}