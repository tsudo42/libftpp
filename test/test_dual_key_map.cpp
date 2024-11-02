#include "data_structures/dual_key_map.hpp"
#include <cassert>
#include <iostream>

void test_insertion_and_access()
{
    DualKeyMap<int, std::string, double> map;

    auto res1 = map.insert(1, "one", 1.1);
    assert(res1.second == true);

    auto res2 = map.insert(2, "two", 2.2);
    assert(res2.second == true);

    auto it1 = map.find_by_key1(1);
    assert(it1 != map.end());
    assert(it1->first.first == 1);
    assert(it1->first.second == "one");
    assert(it1->second == 1.1);

    auto it2 = map.find_by_key2("two");
    assert(it2 != map.end());
    assert(it2->first.first == 2);
    assert(it2->first.second == "two");
    assert(it2->second == 2.2);

    auto it3 = map.find(1);
    assert(it3 != map.end());
    assert(it3->first.first == 1);
    assert(it3->first.second == "one");
    assert(it3->second == 1.1);

    auto it4 = map.find("two");
    assert(it4 != map.end());
    assert(it4->first.first == 2);
    assert(it4->first.second == "two");
    assert(it4->second == 2.2);

    std::cout << "Test1: Insertion and Access - Passed" << std::endl;
}

void test_duplicate_insertion()
{
    DualKeyMap<int, std::string, double> map;

    auto res1 = map.insert(1, "one", 1.1);
    assert(res1.second == true);

    auto res2 = map.insert(1, "one", 1.2);
    assert(res2.second == false);

    auto it4 = map.find("one");
    assert(it4 != map.end());
    assert(it4->second == 1.1);

    std::cout << "Test2: Duplicate Insertion - Passed" << std::endl;
}

void test_erase_by_key1()
{
    DualKeyMap<int, std::string, double> map;

    map.insert(1, "one", 1.1);
    map.insert(2, "two", 2.2);

    size_t erased = map.erase_by_key1(1);
    assert(erased == 1);
    assert(map.find_by_key1(1) == map.end());
    assert(map.find_by_key2("one") == map.end());

    std::cout << "Test3: Erase by Key1 - Passed" << std::endl;
}

void test_erase_by_key2()
{
    DualKeyMap<int, std::string, double> map;

    map.insert(1, "one", 1.1);
    map.insert(2, "two", 2.2);

    size_t erased = map.erase_by_key2("two");
    assert(erased == 1);
    assert(map.find_by_key1(2) == map.end());
    assert(map.find_by_key2("two") == map.end());

    std::cout << "Test4: Erase by Key2 - Passed" << std::endl;
}

void test_empty_and_size()
{
    DualKeyMap<int, std::string, double> map;
    assert(map.empty());
    assert(map.size() == 0);

    map.insert(1, "one", 1.1);
    assert(!map.empty());
    assert(map.size() == 1);

    map.clear();
    assert(map.empty());
    assert(map.size() == 0);

    std::cout << "Test5: Empty and Size - Passed" << std::endl;
}

void test_iteration()
{
    DualKeyMap<int, std::string, double> map;

    map.insert(1, "one", 1.1);
    map.insert(2, "two", 2.2);
    map.insert(3, "three", 3.3);

    int count = 0;
    for (auto it = map.begin(); it != map.end(); ++it)
    {
        count++;
        assert(it->second == 1.1 || it->second == 2.2 || it->second == 3.3);
    }
    assert(count == 3);

    std::cout << "Test6: Iteration - Passed" << std::endl;
}

int main()
{
    test_insertion_and_access();
    test_duplicate_insertion();
    test_erase_by_key1();
    test_erase_by_key2();
    test_empty_and_size();
    test_iteration();

    std::cout << "Finished all tests." << std::endl;
    return 0;
}
