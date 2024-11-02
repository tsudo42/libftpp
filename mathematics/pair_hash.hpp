#ifndef PAIR_HASH_HPP
#define PAIR_HASH_HPP

#include <functional>

const std::size_t GOLDEN_RATIO = 0x9e3779b9;

template <typename TType>
inline void hash_combine(std::size_t &seed, const TType &v)
{
    std::hash<TType> hasher;
    seed ^= hasher(v) + GOLDEN_RATIO + (seed << 6) + (seed >> 2);
}

template <typename Key1, typename Key2>
struct pair_hash
{
    std::size_t operator()(const std::pair<Key1, Key2> &pair) const
    {
        std::size_t seed = 0;
        hash_combine(seed, pair.first);
        hash_combine(seed, pair.second);
        return seed;
    }
};

#endif /* PAIR_HASH_HPP */
