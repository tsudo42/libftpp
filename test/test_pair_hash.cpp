#include "mathematics/pair_hash.hpp"
#include <cassert>
#include <iostream>
#include <utility>
#include <unordered_map>

void test_pair_hash_basic()
{
    pair_hash<int, int> hasher;
    std::pair<int, int> pair1 = {1, 2};
    std::pair<int, int> pair2 = {1, 2};
    std::pair<int, int> pair3 = {2, 1};

    assert(hasher(pair1) == hasher(pair2)); // Same pairs should have the same hash
    assert(hasher(pair1) != hasher(pair3)); // Different pairs should have different hashes

    std::cout << "Test1: Basic pair hash - Passed" << std::endl;
}

void test_pair_hash_with_unordered_map()
{
    std::unordered_map<std::pair<int, int>, std::string, pair_hash<int, int>> map;
    map[{1, 2}] = "one-two";
    map[{2, 1}] = "two-one";

    assert((map[{1, 2}]) == "one-two");
    assert((map[{2, 1}]) == "two-one");

    std::cout << "Test2: Pair hash with unordered_map - Passed" << std::endl;
}

void test_pair_hash_different_types()
{
    pair_hash<int, std::string> hasher;
    std::pair<int, std::string> pair1 = {1, "one"};
    std::pair<int, std::string> pair2 = {1, "one"};
    std::pair<int, std::string> pair3 = {2, "two"};

    assert(hasher(pair1) == hasher(pair2)); // Same pairs should have the same hash
    assert(hasher(pair1) != hasher(pair3)); // Different pairs should have different hashes

    std::cout << "Test3: Pair hash with different types - Passed" << std::endl;
}

int main()
{
    test_pair_hash_basic();
    test_pair_hash_with_unordered_map();
    test_pair_hash_different_types();

    std::cout << "Finished all tests." << std::endl;
    return 0;
}
