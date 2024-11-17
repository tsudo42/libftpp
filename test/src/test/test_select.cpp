#include "misc/select.hpp"
#include <iostream>
#include <type_traits>

void test_select()
{
    static_assert(std::is_same<ft::select<0, int, double, char>::type, int>::value, "Test1 failed");
    static_assert(std::is_same<ft::select<1, int, double, char>::type, double>::value, "Test2 failed");
    static_assert(std::is_same<ft::select<2, int, double, char>::type, char>::value, "Test3 failed");

    std::cout << "Test1: default select - Passed" << std::endl;
}

void test_alias()
{
    static_assert(std::is_same<ft::Select<0, int, double, char>, int>::value, "Test1 failed");
    static_assert(std::is_same<ft::Select<1, int, double, char>, double>::value, "Test2 failed");
    static_assert(std::is_same<ft::Select<2, int, double, char>, char>::value, "Test3 failed");

    std::cout << "Test2: Select alias - Passed" << std::endl;
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

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
