#include "mathematics/random_2D_coordinate_generator.hpp"

#include <cstdint>
#include <memory>

Random2DCoordinateGenerator::Random2DCoordinateGenerator(long long seed)
    : seed_(seed) {}

namespace
{
    const uint64_t k_val = 0x9ae16a3b2f90404fULL;
    const uint64_t k_mul = 0x9ddfea08eb382d69ULL;

    uint64_t fetch64(const char *p)
    {
        uint64_t result;
        std::memcpy(&result, p, sizeof(uint64_t));
        return result;
    }

    uint64_t rotate(uint64_t v, int s)
    {
        return s == 0 ? v : ((v >> s) | (v << (64 - s)));
    }

    uint64_t HashLen16(uint64_t u, uint64_t v, uint64_t mul)
    {
        uint64_t a = (u ^ v) * mul;
        a ^= (a >> 47);
        uint64_t b = (v ^ a) * mul;
        b ^= (b >> 47);
        b *= mul;
        return b;
    }

    uint64_t HashLen0to16(const char *s, std::size_t len)
    {
        uint64_t mul = k_val + len * 4;
        uint64_t a = fetch64(s) + k_val;
        uint64_t b = fetch64(s + len - 8);
        uint64_t c = rotate(b, 37) * mul + a;
        uint64_t d = (rotate(a, 25) + b) * mul;
        return HashLen16(c, d, mul);
    }

    uint64_t cityhash(uint64_t a, uint64_t b, uint64_t seed)
    {
        char s[sizeof(uint64_t) * 2];
        std::memcpy(s, &a, sizeof(uint64_t));
        std::memcpy(s + sizeof(uint64_t), &b, sizeof(uint64_t));

        return HashLen16(HashLen0to16(s, sizeof(uint64_t) * 2) - k_val, seed, k_mul);
    }
}

long long Random2DCoordinateGenerator::operator()(const long long &x, const long long &y) const
{
    uint64_t h = cityhash(static_cast<uint64_t>(x), static_cast<uint64_t>(y), seed_);

    return static_cast<long long>(h);
}

long long Random2DCoordinateGenerator::seed() const
{
    return seed_;
}

void Random2DCoordinateGenerator::set_seed(const long long &seed)
{
    seed_ = seed;
}
