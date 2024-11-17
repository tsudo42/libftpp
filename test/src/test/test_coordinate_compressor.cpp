#include "data_structures/coordinate_compressor.hpp"

#include <cassert>
#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <set>

void test_constructor_and_insert()
{
    CoordinateCompressor<int> compressor;

    compressor.insert(10);
    compressor.insert(5);
    compressor.insert(20);

    std::vector<std::size_t> compressed = compressor.compress();
    assert(compressed.size() == 3);

    std::cout << "Test1: constructor and insert - Passed" << std::endl;
}

void test_initializer_list_constructor()
{
    CoordinateCompressor<int> compressor = {10, 5, 20};

    std::vector<std::size_t> compressed = compressor.compress();
    assert(compressed.size() == 3);

    std::cout << "Test2: initializer list constructor - Passed" << std::endl;
}

void test_range_constructor()
{
    std::vector<int> values = {10, 5, 20};
    CoordinateCompressor<int> compressor(values.begin(), values.end());

    std::vector<std::size_t> compressed = compressor.compress();
    assert(compressed.size() == 3);

    std::cout << "Test3: range constructor - Passed" << std::endl;
}

void test_compress_unique_values()
{
    CoordinateCompressor<int> compressor;
    compressor.insert(10);
    compressor.insert(5);
    compressor.insert(20);

    std::vector<std::size_t> compressed = compressor.compress();

    assert(compressed[0] == 1);
    assert(compressed[1] == 0);
    assert(compressed[2] == 2);

    std::cout << "Test4: compress unique values - Passed" << std::endl;
}

void test_compress_with_duplicates()
{
    CoordinateCompressor<int> compressor;
    compressor.insert(10);
    compressor.insert(5);
    compressor.insert(20);
    compressor.insert(10);

    std::vector<std::size_t> compressed = compressor.compress();

    assert(compressed.size() == 4);
    assert(compressed[0] == 1);
    assert(compressed[1] == 0);
    assert(compressed[2] == 2);
    assert(compressed[3] == 1);

    std::cout << "Test5: compress with duplicates - Passed" << std::endl;
}

void test_big()
{
    std::vector<int> values = {
        44, 13, 57, 68, 78, 95, 33, 41, 59, 25, 25, 96, 23, 80, 29, 93, 29, 33, 35, 98, 36, 17, 34, 36, 94, 66, 73, 45, 20, 34, 87, 67, 38, 57, 57, 97, 97, 72, 13, 67, 29, 44, 19, 23, 56, 63, 68, 26, 9, 51, 37, 31, 85, 87, 75, 88, 91, 91, 37, 53, 31, 52, 40, 28, 78, 9, 73, 3, 4, 12, 16, 44, 1, 99, 67, 91, 84, 54, 64, 42, 90, 10, 67, 13, 49, 94, 90, 47, 59, 55, 29, 1, 19, 69, 69, 36, 42, 8, 93, 6, 82, 60, 91, 65, 58, 39, 26, 53, 21, 87, 29, 45, 23, 63, 98, 28, 44, 77, 87, 89, 82, 80, 9, 18, 59, 54, 56, 16, 88, 100, 75, 87, 15, 69, 34, 86, 69, 73, 39, 63, 91, 29, 67, 16, 10, 58, 24, 64, 80, 57, 19, 22, 97, 49, 99, 14, 82, 7, 82, 78, 61, 8, 68, 89, 98, 11, 17, 88, 38, 18, 55, 74, 77, 44, 65, 64, 62, 35, 85, 85, 83, 8, 57, 56, 49, 59, 23, 97, 60, 18, 88, 76, 91, 10, 30, 55, 48, 53, 71, 36, 16, 0, 30, 27, 43, 98, 3, 44, 52, 47, 91, 77, 20, 36, 14, 81, 2, 76, 79, 93, 13, 50, 40, 100, 73, 93, 55, 24, 88, 92, 79, 14, 42, 58, 48, 95, 65, 70, 10, 98, 68, 6, 77, 37, 74, 72, 21, 79, 66, 53, 13, 21, 38, 19, 15, 43, 7, 4, 100, 54, 5, 96, 26, 56, 4, 62, 0, 13, 96, 53, 88, 37, 40, 95, 21, 44, 78, 62, 74, 98, 65, 43, 17, 92, 61, 38, 76, 19, 36, 14, 0, 20, 31, 3, 19, 71, 5, 94, 83, 97, 88, 74, 93, 47, 5, 56, 63, 49, 38, 72, 47, 65, 89, 39, 50, 55, 71, 37, 0, 33, 100, 9, 44, 97, 68, 21, 95, 99, 29, 49, 70, 68, 97, 66, 21, 1, 71, 74, 74, 82, 80, 74, 61, 43, 23, 95, 88, 14, 75, 11, 22, 87, 93, 100, 45, 18, 83, 14, 34, 13, 95, 49, 8, 22, 11, 85, 84, 11, 2, 74, 78, 95, 1, 7, 70, 12, 34, 62, 66, 21, 33, 41, 11, 72, 12, 29, 51, 68, 29, 38, 89, 41, 70, 65, 0, 23, 76, 68, 75, 2, 57, 33, 2, 77, 66, 36, 9, 42, 11, 61, 90, 15, 14, 54, 74, 85, 22, 19, 29, 85, 78, 39, 75, 51, 88, 77, 15, 35, 100, 17, 19, 60, 86, 39, 8, 78, 63, 85, 75, 24, 31, 9, 92, 54, 88, 44, 87, 1, 73, 62, 57, 42, 11, 64, 61, 39, 73, 45, 41, 15, 68, 48, 67, 29, 23, 45, 74, 89, 79, 85, 52, 39, 7, 13, 89, 84, 76, 23, 8, 54, 47, 26, 25, 17, 48, 17, 28, 79, 32, 74, 61, 54, 64, 41, 71, 55, 19, 95, 37, 55};
    std::vector<std::size_t> answer = {
        44, 13, 56, 67, 77, 94, 33, 41, 58, 25, 25, 95, 23, 79, 29, 92, 29, 33, 35, 97, 36, 17, 34, 36, 93, 65, 72, 45, 20, 34, 86, 66, 38, 56, 56, 96, 96, 71, 13, 66, 29, 44, 19, 23, 55, 62, 67, 26, 9, 50, 37, 31, 84, 86, 74, 87, 90, 90, 37, 52, 31, 51, 40, 28, 77, 9, 72, 3, 4, 12, 16, 44, 1, 98, 66, 90, 83, 53, 63, 42, 89, 10, 66, 13, 48, 93, 89, 46, 58, 54, 29, 1, 19, 68, 68, 36, 42, 8, 92, 6, 81, 59, 90, 64, 57, 39, 26, 52, 21, 86, 29, 45, 23, 62, 97, 28, 44, 76, 86, 88, 81, 79, 9, 18, 58, 53, 55, 16, 87, 99, 74, 86, 15, 68, 34, 85, 68, 72, 39, 62, 90, 29, 66, 16, 10, 57, 24, 63, 79, 56, 19, 22, 96, 48, 98, 14, 81, 7, 81, 77, 60, 8, 67, 88, 97, 11, 17, 87, 38, 18, 54, 73, 76, 44, 64, 63, 61, 35, 84, 84, 82, 8, 56, 55, 48, 58, 23, 96, 59, 18, 87, 75, 90, 10, 30, 54, 47, 52, 70, 36, 16, 0, 30, 27, 43, 97, 3, 44, 51, 46, 90, 76, 20, 36, 14, 80, 2, 75, 78, 92, 13, 49, 40, 99, 72, 92, 54, 24, 87, 91, 78, 14, 42, 57, 47, 94, 64, 69, 10, 97, 67, 6, 76, 37, 73, 71, 21, 78, 65, 52, 13, 21, 38, 19, 15, 43, 7, 4, 99, 53, 5, 95, 26, 55, 4, 61, 0, 13, 95, 52, 87, 37, 40, 94, 21, 44, 77, 61, 73, 97, 64, 43, 17, 91, 60, 38, 75, 19, 36, 14, 0, 20, 31, 3, 19, 70, 5, 93, 82, 96, 87, 73, 92, 46, 5, 55, 62, 48, 38, 71, 46, 64, 88, 39, 49, 54, 70, 37, 0, 33, 99, 9, 44, 96, 67, 21, 94, 98, 29, 48, 69, 67, 96, 65, 21, 1, 70, 73, 73, 81, 79, 73, 60, 43, 23, 94, 87, 14, 74, 11, 22, 86, 92, 99, 45, 18, 82, 14, 34, 13, 94, 48, 8, 22, 11, 84, 83, 11, 2, 73, 77, 94, 1, 7, 69, 12, 34, 61, 65, 21, 33, 41, 11, 71, 12, 29, 50, 67, 29, 38, 88, 41, 69, 64, 0, 23, 75, 67, 74, 2, 56, 33, 2, 76, 65, 36, 9, 42, 11, 60, 89, 15, 14, 53, 73, 84, 22, 19, 29, 84, 77, 39, 74, 50, 87, 76, 15, 35, 99, 17, 19, 59, 85, 39, 8, 77, 62, 84, 74, 24, 31, 9, 91, 53, 87, 44, 86, 1, 72, 61, 56, 42, 11, 63, 60, 39, 72, 45, 41, 15, 67, 47, 66, 29, 23, 45, 73, 88, 78, 84, 51, 39, 7, 13, 88, 83, 75, 23, 8, 53, 46, 26, 25, 17, 47, 17, 28, 78, 32, 73, 60, 53, 63, 41, 70, 54, 19, 94, 37, 54};

    CoordinateCompressor<int> compressor(values.begin(), values.end());
    std::vector<std::size_t> compressed = compressor.compress();
    assert(compressed == answer);

    std::cout << "Test6: big - Passed" << std::endl;
}

void test_random()
{
    std::mt19937 gen(42);
    std::uniform_int_distribution<> dist(1, 10000000);
    std::vector<int> random_data(5000);
    for (auto &val : random_data)
    {
        val = dist(gen);
    }

    CoordinateCompressor<int> compressor(random_data.begin(), random_data.end());
    std::vector<std::size_t> result = compressor.compress();

    std::set<int> unique_values(random_data.begin(), random_data.end());
    std::size_t unique_size = unique_values.size();

    auto min_max = std::minmax_element(result.begin(), result.end());
    assert(*min_max.first == 0);
    assert(*min_max.second == unique_size - 1);

    for (std::size_t i = 0; i < random_data.size(); ++i)
    {
        for (std::size_t j = i + 1; j < random_data.size(); ++j)
        {
            if (random_data[i] == random_data[j])
            {
                assert(result[i] == result[j]);
            }
            else if (random_data[i] < random_data[j])
            {
                assert(result[i] < result[j]);
            }
            else
            {
                assert(result[i] > result[j]);
            }
        }
    }

    std::cout << "Test7: random - Passed" << std::endl;
}

int main()
{
    test_constructor_and_insert();
    test_initializer_list_constructor();
    test_range_constructor();
    test_compress_unique_values();
    test_compress_with_duplicates();
    test_big();
    test_random();

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
