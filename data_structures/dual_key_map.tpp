#ifndef DUAL_KEY_MAP_TPP
#define DUAL_KEY_MAP_TPP

#include "data_structures/dual_key_map.hpp"
#include <type_traits>

template <typename Key1, typename Key2, typename Value, typename Hash, typename KeyEqual>
typename DualKeyMap<Key1, Key2, Value, Hash, KeyEqual>::iterator
DualKeyMap<Key1, Key2, Value, Hash, KeyEqual>::begin() noexcept
{
    return values_.begin();
}

template <typename Key1, typename Key2, typename Value, typename Hash, typename KeyEqual>
typename DualKeyMap<Key1, Key2, Value, Hash, KeyEqual>::const_iterator
DualKeyMap<Key1, Key2, Value, Hash, KeyEqual>::begin() const noexcept
{
    return values_.begin();
}

template <typename Key1, typename Key2, typename Value, typename Hash, typename KeyEqual>
typename DualKeyMap<Key1, Key2, Value, Hash, KeyEqual>::const_iterator
DualKeyMap<Key1, Key2, Value, Hash, KeyEqual>::cbegin() const noexcept
{
    return values_.cbegin();
}

template <typename Key1, typename Key2, typename Value, typename Hash, typename KeyEqual>
typename DualKeyMap<Key1, Key2, Value, Hash, KeyEqual>::iterator
DualKeyMap<Key1, Key2, Value, Hash, KeyEqual>::end() noexcept
{
    return values_.end();
}

template <typename Key1, typename Key2, typename Value, typename Hash, typename KeyEqual>
typename DualKeyMap<Key1, Key2, Value, Hash, KeyEqual>::const_iterator
DualKeyMap<Key1, Key2, Value, Hash, KeyEqual>::end() const noexcept
{
    return values_.end();
}

template <typename Key1, typename Key2, typename Value, typename Hash, typename KeyEqual>
typename DualKeyMap<Key1, Key2, Value, Hash, KeyEqual>::const_iterator
DualKeyMap<Key1, Key2, Value, Hash, KeyEqual>::cend() const noexcept
{
    return values_.cend();
}

template <typename Key1, typename Key2, typename Value, typename Hash, typename KeyEqual>
bool DualKeyMap<Key1, Key2, Value, Hash, KeyEqual>::empty() const noexcept
{
    return values_.empty();
}

template <typename Key1, typename Key2, typename Value, typename Hash, typename KeyEqual>
typename DualKeyMap<Key1, Key2, Value, Hash, KeyEqual>::size_type
DualKeyMap<Key1, Key2, Value, Hash, KeyEqual>::size() const noexcept
{
    return values_.size();
}

template <typename Key1, typename Key2, typename Value, typename Hash, typename KeyEqual>
void DualKeyMap<Key1, Key2, Value, Hash, KeyEqual>::clear() noexcept
{
    values_.clear();
    key1_to_key2_.clear();
    key2_to_key1_.clear();
}

template <typename Key1, typename Key2, typename Value, typename Hash, typename KeyEqual>
std::pair<typename DualKeyMap<Key1, Key2, Value, Hash, KeyEqual>::iterator, bool>
DualKeyMap<Key1, Key2, Value, Hash, KeyEqual>::insert(const value_type &value)
{
    const auto &key_pair = value.first;
    const Key1 &key1 = key_pair.first;
    const Key2 &key2 = key_pair.second;

    if (key1_to_key2_.count(key1) > 0 || key2_to_key1_.count(key2) > 0)
    {
        return {values_.end(), false};
    }

    auto result = values_.insert(value);
    if (result.second)
    {
        key1_to_key2_.emplace(key1, key2);
        key2_to_key1_.emplace(key2, key1);
    }

    return result;
}

template <typename Key1, typename Key2, typename Value, typename Hash, typename KeyEqual>
std::pair<typename DualKeyMap<Key1, Key2, Value, Hash, KeyEqual>::iterator, bool>
DualKeyMap<Key1, Key2, Value, Hash, KeyEqual>::insert(value_type &&value)
{
    const auto &key_pair = value.first;
    const Key1 &key1 = key_pair.first;
    const Key2 &key2 = key_pair.second;

    if (key1_to_key2_.count(key1) > 0 || key2_to_key1_.count(key2) > 0)
    {
        return {values_.end(), false};
    }

    auto result = values_.insert(std::move(value));
    if (result.second)
    {
        key1_to_key2_.emplace(key1, key2);
        key2_to_key1_.emplace(key2, key1);
    }

    return result;
}

template <typename Key1, typename Key2, typename Value, typename Hash, typename KeyEqual>
std::pair<typename DualKeyMap<Key1, Key2, Value, Hash, KeyEqual>::iterator, bool>
DualKeyMap<Key1, Key2, Value, Hash, KeyEqual>::insert(const Key1 &key1, const Key2 &key2, const Value &value)
{
    if (key1_to_key2_.count(key1) > 0 || key2_to_key1_.count(key2) > 0)
    {
        return {values_.end(), false};
    }

    auto result = values_.insert({std::make_pair(key1, key2), value});
    if (result.second)
    {
        key1_to_key2_.emplace(key1, key2);
        key2_to_key1_.emplace(key2, key1);
    }

    return result;
}

template <typename Key1, typename Key2, typename Value, typename Hash, typename KeyEqual>
std::pair<typename DualKeyMap<Key1, Key2, Value, Hash, KeyEqual>::iterator, bool>
DualKeyMap<Key1, Key2, Value, Hash, KeyEqual>::insert(Key1 &&key1, Key2 &&key2, Value &&value)
{
    if (key1_to_key2_.count(key1) > 0 || key2_to_key1_.count(key2) > 0)
    {
        return {values_.end(), false};
    }

    auto result = values_.insert({std::make_pair(std::forward<Key1>(key1), std::forward<Key2>(key2)), std::forward<Value>(value)});
    if (result.second)
    {
        key1_to_key2_.emplace(result.first->first.first, result.first->first.second);
        key2_to_key1_.emplace(result.first->first.second, result.first->first.first);
    }

    return result;
}

template <typename Key1, typename Key2, typename Value, typename Hash, typename KeyEqual>
template <typename... TArgs>
std::pair<typename DualKeyMap<Key1, Key2, Value, Hash, KeyEqual>::iterator, bool>
DualKeyMap<Key1, Key2, Value, Hash, KeyEqual>::emplace(TArgs &&...args)
{
    value_type value(std::forward<TArgs>(args)...);
    const auto &key_pair = value.first;
    const Key1 &key1 = key_pair.first;
    const Key2 &key2 = key_pair.second;

    if (key1_to_key2_.count(key1) > 0 || key2_to_key1_.count(key2) > 0)
    {
        return {values_.end(), false};
    }

    auto result = values_.emplace(std::move(value));
    if (result.second)
    {
        key1_to_key2_.emplace(key1, key2);
        key2_to_key1_.emplace(key2, key1);
    }

    return result;
}

template <typename Key1, typename Key2, typename Value, typename Hash, typename KeyEqual>
template <typename T1, typename T2, typename... TArgs>
std::pair<typename DualKeyMap<Key1, Key2, Value, Hash, KeyEqual>::iterator, bool>
DualKeyMap<Key1, Key2, Value, Hash, KeyEqual>::emplace(T1 &&key1, T2 &&key2, TArgs &&...args)
{
    if (key1_to_key2_.count(key1) > 0 || key2_to_key1_.count(key2) > 0)
    {
        return {values_.end(), false};
    }

    auto result = values_.emplace(
        std::make_pair(std::forward<T1>(key1), std::forward<T2>(key2)),
        Value(std::forward<TArgs>(args)...));

    if (result.second)
    {
        key1_to_key2_.emplace(result.first->first.first, result.first->first.second);
        key2_to_key1_.emplace(result.first->first.second, result.first->first.first);
    }

    return result;
}

template <typename Key1, typename Key2, typename Value, typename Hash, typename KeyEqual>
typename DualKeyMap<Key1, Key2, Value, Hash, KeyEqual>::iterator
DualKeyMap<Key1, Key2, Value, Hash, KeyEqual>::erase(iterator pos)
{
    if (pos == values_.end())
    {
        return pos;
    }

    const auto &key_pair = pos->first;
    const Key1 &key1 = key_pair.first;
    const Key2 &key2 = key_pair.second;

    key1_to_key2_.erase(key1);
    key2_to_key1_.erase(key2);

    return values_.erase(pos);
}

template <typename Key1, typename Key2, typename Value, typename Hash, typename KeyEqual>
typename DualKeyMap<Key1, Key2, Value, Hash, KeyEqual>::iterator
DualKeyMap<Key1, Key2, Value, Hash, KeyEqual>::erase(const_iterator pos)
{
    if (pos == values_.cend())
    {
        return values_.end();
    }

    const auto &key_pair = pos->first;
    const Key1 &key1 = key_pair.first;
    const Key2 &key2 = key_pair.second;

    key1_to_key2_.erase(key1);
    key2_to_key1_.erase(key2);

    return values_.erase(pos);
}

template <typename Key1, typename Key2, typename Value, typename Hash, typename KeyEqual>
template <typename T, typename>
typename DualKeyMap<Key1, Key2, Value, Hash, KeyEqual>::size_type
DualKeyMap<Key1, Key2, Value, Hash, KeyEqual>::erase(const key1_type &key1)
{
    static_assert(!is_equivalent<Key1, Key2>::value, "Cannot identify which key to erase. Use erase_by_key1 or erase_by_key2 instead.");
    return erase_by_key1(key1);
}

template <typename Key1, typename Key2, typename Value, typename Hash, typename KeyEqual>
template <typename T, typename>
typename DualKeyMap<Key1, Key2, Value, Hash, KeyEqual>::size_type
DualKeyMap<Key1, Key2, Value, Hash, KeyEqual>::erase(const key2_type &key2)
{
    static_assert(!is_equivalent<Key1, Key2>::value, "Cannot identify which key to erase. Use erase_by_key1 or erase_by_key2 instead.");
    return erase_by_key2(key2);
}

template <typename Key1, typename Key2, typename Value, typename Hash, typename KeyEqual>
typename DualKeyMap<Key1, Key2, Value, Hash, KeyEqual>::size_type
DualKeyMap<Key1, Key2, Value, Hash, KeyEqual>::erase_by_key1(const key1_type &key1)
{
    auto it_key1 = key1_to_key2_.find(key1);
    if (it_key1 == key1_to_key2_.end())
    {
        return 0;
    }

    Key2 key2 = it_key1->second;
    values_.erase({key1, key2});
    key1_to_key2_.erase(it_key1);
    key2_to_key1_.erase(key2);

    return 1;
}

template <typename Key1, typename Key2, typename Value, typename Hash, typename KeyEqual>
typename DualKeyMap<Key1, Key2, Value, Hash, KeyEqual>::size_type
DualKeyMap<Key1, Key2, Value, Hash, KeyEqual>::erase_by_key2(const key2_type &key2)
{
    auto it_key2 = key2_to_key1_.find(key2);
    if (it_key2 == key2_to_key1_.end())
    {
        return 0;
    }

    Key1 key1 = it_key2->second;
    values_.erase({key1, key2});
    key2_to_key1_.erase(it_key2);
    key1_to_key2_.erase(key1);

    return 1;
}

template <typename Key1, typename Key2, typename Value, typename Hash, typename KeyEqual>
void DualKeyMap<Key1, Key2, Value, Hash, KeyEqual>::swap(DualKeyMap &other)
{
    values_.swap(other.values_);
    key1_to_key2_.swap(other.key1_to_key2_);
    key2_to_key1_.swap(other.key2_to_key1_);
}

template <typename Key1, typename Key2, typename Value, typename Hash, typename KeyEqual>
template <typename T, typename>
typename DualKeyMap<Key1, Key2, Value, Hash, KeyEqual>::size_type
DualKeyMap<Key1, Key2, Value, Hash, KeyEqual>::count(const key1_type &key1) const
{
    static_assert(!is_equivalent<Key1, Key2>::value, "Cannot identify which key to count. Use count_by_key1 or count_by_key2 instead.");

    return key1_to_key2_.count(key1);
}

template <typename Key1, typename Key2, typename Value, typename Hash, typename KeyEqual>
template <typename T, typename>
typename DualKeyMap<Key1, Key2, Value, Hash, KeyEqual>::size_type
DualKeyMap<Key1, Key2, Value, Hash, KeyEqual>::count(const key2_type &key2) const
{
    static_assert(!is_equivalent<Key1, Key2>::value, "Cannot identify which key to count. Use count_by_key1 or count_by_key2 instead.");

    return key2_to_key1_.count(key2);
}

template <typename Key1, typename Key2, typename Value, typename Hash, typename KeyEqual>
typename DualKeyMap<Key1, Key2, Value, Hash, KeyEqual>::size_type
DualKeyMap<Key1, Key2, Value, Hash, KeyEqual>::count_by_key1(const key1_type &key1) const
{
    return key1_to_key2_.count(key1);
}

template <typename Key1, typename Key2, typename Value, typename Hash, typename KeyEqual>
typename DualKeyMap<Key1, Key2, Value, Hash, KeyEqual>::size_type
DualKeyMap<Key1, Key2, Value, Hash, KeyEqual>::count_by_key2(const key2_type &key2) const
{
    return key2_to_key1_.count(key2);
}

template <typename Key1, typename Key2, typename Value, typename Hash, typename KeyEqual>
template <typename T, typename>
typename DualKeyMap<Key1, Key2, Value, Hash, KeyEqual>::iterator
DualKeyMap<Key1, Key2, Value, Hash, KeyEqual>::find(const key1_type &key1)
{
    static_assert(!is_equivalent<Key1, Key2>::value, "Cannot identify which key to find. Use find_by_key1 or find_by_key2 instead.");

    return find_by_key1(key1);
}

template <typename Key1, typename Key2, typename Value, typename Hash, typename KeyEqual>
template <typename T, typename>
typename DualKeyMap<Key1, Key2, Value, Hash, KeyEqual>::const_iterator
DualKeyMap<Key1, Key2, Value, Hash, KeyEqual>::find(const key1_type &key1) const
{
    static_assert(!is_equivalent<Key1, Key2>::value, "Cannot identify which key to find. Use find_by_key1 or find_by_key2 instead.");

    return find_by_key1(key1);
}

template <typename Key1, typename Key2, typename Value, typename Hash, typename KeyEqual>
typename DualKeyMap<Key1, Key2, Value, Hash, KeyEqual>::iterator
DualKeyMap<Key1, Key2, Value, Hash, KeyEqual>::find_by_key1(const key1_type &key1)
{
    auto it_key1 = key1_to_key2_.find(key1);
    if (it_key1 == key1_to_key2_.end())
    {
        return values_.end();
    }

    const Key2 &key2 = it_key1->second;
    return values_.find({key1, key2});
}

template <typename Key1, typename Key2, typename Value, typename Hash, typename KeyEqual>
typename DualKeyMap<Key1, Key2, Value, Hash, KeyEqual>::const_iterator
DualKeyMap<Key1, Key2, Value, Hash, KeyEqual>::find_by_key1(const key1_type &key1) const
{
    auto it_key1 = key1_to_key2_.find(key1);
    if (it_key1 == key1_to_key2_.end())
    {
        return values_.end();
    }

    const Key2 &key2 = it_key1->second;
    return values_.find({key1, key2});
}

template <typename Key1, typename Key2, typename Value, typename Hash, typename KeyEqual>
template <typename T, typename>
typename DualKeyMap<Key1, Key2, Value, Hash, KeyEqual>::iterator
DualKeyMap<Key1, Key2, Value, Hash, KeyEqual>::find(const key2_type &key2)
{
    static_assert(!is_equivalent<Key1, Key2>::value, "Cannot identify which key to find. Use find_by_key1 or find_by_key2 instead.");

    return find_by_key2(key2);
}

template <typename Key1, typename Key2, typename Value, typename Hash, typename KeyEqual>
template <typename T, typename>
typename DualKeyMap<Key1, Key2, Value, Hash, KeyEqual>::const_iterator
DualKeyMap<Key1, Key2, Value, Hash, KeyEqual>::find(const key2_type &key2) const
{
    static_assert(!is_equivalent<Key1, Key2>::value, "Cannot identify which key to find. Use find_by_key1 or find_by_key2 instead.");

    return find_by_key2(key2);
}

template <typename Key1, typename Key2, typename Value, typename Hash, typename KeyEqual>
typename DualKeyMap<Key1, Key2, Value, Hash, KeyEqual>::iterator
DualKeyMap<Key1, Key2, Value, Hash, KeyEqual>::find_by_key2(const key2_type &key2)
{
    auto it_key2 = key2_to_key1_.find(key2);
    if (it_key2 == key2_to_key1_.end())
    {
        return values_.end();
    }

    const Key1 &key1 = it_key2->second;
    return values_.find({key1, key2});
}

template <typename Key1, typename Key2, typename Value, typename Hash, typename KeyEqual>
typename DualKeyMap<Key1, Key2, Value, Hash, KeyEqual>::const_iterator
DualKeyMap<Key1, Key2, Value, Hash, KeyEqual>::find_by_key2(const key2_type &key2) const
{
    auto it_key2 = key2_to_key1_.find(key2);
    if (it_key2 == key2_to_key1_.end())
    {
        return values_.end();
    }

    const Key1 &key1 = it_key2->second;
    return values_.find({key1, key2});
}

#endif /* DUAL_KEY_MAP_TPP */
