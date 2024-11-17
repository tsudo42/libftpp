#include "mathematics/angle.hpp"

#include <cassert>
#include <iostream>
#include <cmath>
#include <sstream>

const double TOLERANCE = 1e-9;

void test_constructor()
{
    Angle a1(180.0);
    assert(std::fabs(a1.degrees() - 180.0) < TOLERANCE);
    assert(std::fabs(a1.radians() - Angle::to_radians(180.0)) < TOLERANCE);

    Angle a2(Angle::PI, false);
    assert(std::fabs(a2.degrees() - 180.0) < TOLERANCE);
    assert(std::fabs(a2.radians() - Angle::PI) < TOLERANCE);

    std::cout << "Test1: constructor - Passed" << std::endl;
}

void test_normalization()
{
    Angle a(370.0);
    a.normalize();
    assert(std::fabs(a.degrees() - 10.0) < TOLERANCE);

    Angle b(-45.0);
    b.normalize();
    assert(std::fabs(b.degrees() - 315.0) < TOLERANCE);

    std::cout << "Test2: normalization - Passed" << std::endl;
}

void test_conversion()
{
    assert(std::fabs(Angle::to_radians(180.0) - Angle::PI) < TOLERANCE);
    assert(std::fabs(Angle::to_degrees(Angle::PI) - 180.0) < TOLERANCE);

    std::cout << "Test3: conversion - Passed" << std::endl;
}

void test_arithmetic_operations()
{
    Angle a(30.0);
    Angle b(45.0);

    Angle result = a + b;
    assert(std::fabs(result.degrees() - 75.0) < TOLERANCE);

    result = a - b;
    assert(std::fabs(result.degrees() - (345.0)) < TOLERANCE);

    a += b;
    assert(std::fabs(a.degrees() - 75.0) < TOLERANCE);

    a -= b;
    assert(std::fabs(a.degrees() - 30.0) < TOLERANCE);

    std::cout << "Test4: arithmetic operations - Passed" << std::endl;
}

void test_complement()
{
    Angle a(30.0);
    Angle comp = a.complement();
    assert(std::fabs(comp.degrees() - 330.0) < TOLERANCE);

    std::cout << "Test5: complement - Passed" << std::endl;
}

void test_wrap_around()
{
    Angle a(400.0);
    a.wrap_around(0.0, 360.0);
    assert(std::fabs(a.degrees() - 40.0) < TOLERANCE);

    Angle b(-400.0);
    b.wrap_around(0.0, 360.0);
    assert(std::fabs(b.degrees() - 320.0) < TOLERANCE);

    std::cout << "Test6: wrap around - Passed" << std::endl;
}

void test_distance()
{
    Angle a(10.0);
    Angle b(350.0);
    assert(std::fabs(a.distance_to(b) - 20.0) < TOLERANCE);

    std::cout << "Test7: distance - Passed" << std::endl;
}

void test_comparison()
{
    Angle a(90.0);
    Angle b(270.0);
    assert(a.is_clockwise_to(b));
    assert(b.is_counterclockwise_to(a));

    Angle c(90.0);
    assert(a == c);

    std::cout << "Test8: comparison - Passed" << std::endl;
}

void test_unit_conversion()
{
    Angle a(180.0);
    a.convert_to_radians();
    assert(std::fabs(a.radians() - Angle::PI) < TOLERANCE);

    a.convert_to_degrees();
    assert(std::fabs(a.degrees() - 180.0) < TOLERANCE);

    std::cout << "Test9: unit conversion - Passed" << std::endl;
}

void test_output()
{
    std::stringstream ss;

    Angle a(90.0);
    ss << a;
    assert(ss.str() == "90°");

    ss.str("");
    a.convert_to_radians();
    ss << a;
    assert(ss.str() == "1.5708 rad");

    std::cout << "Test10: output - Passed" << std::endl;
}

int main()
{
    test_constructor();
    test_normalization();
    test_conversion();
    test_arithmetic_operations();
    test_complement();
    test_wrap_around();
    test_distance();
    test_comparison();
    test_unit_conversion();
    test_output();

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
