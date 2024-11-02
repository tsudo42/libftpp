#ifndef DUAL_KEY_MAP_HPP
#define DUAL_KEY_MAP_HPP

#include <list>
#include <unordered_map>
#include <type_traits>

#include "mathematics/pair_hash.hpp"

template <class TType>
using remove_cvref_t = std::remove_cv_t<std::remove_reference_t<TType>>;

template <typename TType1, typename TType2>
using is_equivalent = std::is_same<remove_cvref_t<TType1>, remove_cvref_t<TType2>>;

template <
    typename Key1,
    typename Key2,
    typename Value,
    typename Hash = pair_hash<Key1, Key2>,
    typename KeyEqual = std::equal_to<std::pair<Key1, Key2>>>
class DualKeyMap
{
public:
    typedef std::pair<Key1, Key2> key_type;
    typedef Key1 key1_type;
    typedef Key2 key2_type;
    typedef std::pair<std::pair<Key1, Key2>, Value> value_type;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;
    typedef Hash hasher;
    typedef KeyEqual key_equal;
    typedef value_type &reference;
    typedef const value_type &const_reference;
    typedef typename std::unordered_map<std::pair<Key1, Key2>, Value, pair_hash<Key1, Key2>>::iterator iterator;
    typedef typename std::unordered_map<std::pair<Key1, Key2>, Value, pair_hash<Key1, Key2>>::const_iterator const_iterator;

    class DuplicateKeyException;

    DualKeyMap() = default;
    DualKeyMap(const DualKeyMap &other) = default;
    DualKeyMap(DualKeyMap &&other) = default;
    ~DualKeyMap() = default;
    DualKeyMap &operator=(const DualKeyMap &other) = default;
    DualKeyMap &operator=(DualKeyMap &&other) = default;

    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    const_iterator cbegin() const noexcept;

    iterator end() noexcept;
    const_iterator end() const noexcept;
    const_iterator cend() const noexcept;

    bool empty() const noexcept;

    size_type size() const noexcept;

    void clear() noexcept;

    std::pair<iterator, bool> insert(const value_type &value);
    std::pair<iterator, bool> insert(value_type &&value);
    std::pair<iterator, bool> insert(const Key1 &key1, const Key2 &key2, const Value &value);
    std::pair<iterator, bool> insert(Key1 &&key1, Key2 &&key2, Value &&value);

    template <typename... TArgs>
    std::pair<iterator, bool> emplace(TArgs &&...args);
    template <typename T1, typename T2, typename... TArgs>
    std::pair<iterator, bool> emplace(T1 &&key1, T2 &&key2, TArgs &&...args);

    iterator erase(iterator pos);
    iterator erase(const_iterator pos);
    template <typename T = Key1, typename = std::enable_if_t<!is_equivalent<T, Key2>::value>>
    size_type erase(const key1_type &key1);
    template <typename T = Key2, typename = std::enable_if_t<!is_equivalent<T, Key1>::value>>
    size_type erase(const key2_type &key2);
    size_type erase_by_key1(const key1_type &key1);
    size_type erase_by_key2(const key2_type &key2);

    void swap(DualKeyMap &other);

    template <typename T = Key1, typename = std::enable_if_t<!is_equivalent<T, Key2>::value>>
    size_type count(const key1_type &key1) const;
    template <typename T = Key2, typename = std::enable_if_t<!is_equivalent<T, Key1>::value>>
    size_type count(const key2_type &key2) const;
    size_type count_by_key1(const key1_type &key1) const;
    size_type count_by_key2(const key2_type &key2) const;

    template <typename T = Key1, typename = std::enable_if_t<!is_equivalent<T, Key2>::value>>
    iterator find(const key1_type &key1);
    template <typename T = Key2, typename = std::enable_if_t<!is_equivalent<T, Key1>::value>>
    iterator find(const key2_type &key2);
    iterator find_by_key1(const key1_type &key1);
    iterator find_by_key2(const key2_type &key2);

    template <typename T = Key1, typename = std::enable_if_t<!is_equivalent<T, Key2>::value>>
    const_iterator find(const key1_type &key1) const;
    template <typename T = Key2, typename = std::enable_if_t<!is_equivalent<T, Key1>::value>>
    const_iterator find(const key2_type &key2) const;
    const_iterator find_by_key1(const key1_type &key1) const;
    const_iterator find_by_key2(const key2_type &key2) const;

private:
    std::unordered_map<key_type, Value, hasher, key_equal> values_;
    std::unordered_map<Key1, Key2> key1_to_key2_;
    std::unordered_map<Key2, Key1> key2_to_key1_;
};

#include "dual_key_map.tpp"

#endif /* DUAL_KEY_MAP_HPP */
