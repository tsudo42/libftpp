#include "libftpp.hpp"

#include <iostream>
#include <cassert>
#include <cstring>

struct TestStruct
{
    int32_t a;
    int16_t b;
    int8_t c;
    uint64_t d;

    bool operator==(const TestStruct &other) const
    {
        return a == other.a && b == other.b && c == other.c && d == other.d;
    }
};

void test_integer()
{
    DataBuffer buffer;

    int32_t intValIn = 0x12345678;
    buffer << intValIn;

    int32_t intValOut = 0;
    buffer >> intValOut;

    assert(intValIn == intValOut);

    std::cout << "Test1: integer - Passed" << std::endl;
}

void test_16bit_integer()
{
    DataBuffer buffer;

    uint16_t shortValIn = 0xABCD;
    buffer << shortValIn;

    uint16_t shortValOut = 0;
    buffer >> shortValOut;

    assert(shortValIn == shortValOut);

    std::cout << "Test2: 16-bit integer - Passed" << std::endl;
}

void test_struct()
{
    DataBuffer buffer;

    TestStruct structValIn = {0x12345678, static_cast<int16_t>(0xABCD), static_cast<int8_t>(0xEF), 0x123456789ABCDEF0};
    buffer << structValIn;

    TestStruct structValOut;
    buffer >> structValOut;

    assert(structValIn == structValOut);

    std::cout << "Test3: struct - Passed" << std::endl;
}

void test_aligned_struct()
{
    DataBuffer buffer;

    struct AlignedStruct
    {
        alignas(16) uint64_t x;
        alignas(16) uint32_t y;
    };

    AlignedStruct alignedValIn = {0x123456789ABCDEF0, 0x12345678};
    buffer << alignedValIn;

    AlignedStruct alignedValOut;
    buffer >> alignedValOut;

    assert(alignedValIn.x == alignedValOut.x);
    assert(alignedValIn.y == alignedValOut.y);

    std::cout << "Test4: aligned struct - Passed" << std::endl;
}

int main()
{
    test_integer();
    test_16bit_integer();
    test_struct();
    test_aligned_struct();

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
