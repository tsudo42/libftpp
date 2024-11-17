#ifndef ENDIAN_HPP
#define ENDIAN_HPP

#include <cstdint>
#include <arpa/inet.h>

#if defined(_DEFAULT_SOURCE) || defined(_BSD_SOURCE)
#include <endian.h>
#endif

template <typename T>
struct Endian;

template <>
struct Endian<uint16_t>
{
    static constexpr uint16_t toNetwork(const uint16_t &value)
    {
        return htons(value);
    }

    static constexpr uint16_t fromNetwork(const uint16_t &value)
    {
        return ntohs(value);
    }
};

template <>
struct Endian<uint32_t>
{
    static constexpr uint32_t toNetwork(const uint32_t &value)
    {
        return htonl(value);
    }

    static constexpr uint32_t fromNetwork(const uint32_t &value)
    {
        return ntohl(value);
    }
};

template <>
struct Endian<uint64_t>
{
    static constexpr uint64_t toNetwork(const uint64_t &value)
    {
#if defined(htonll)
        return htonll(value);
#elif defined(htobe64)
        return htobe64(value);
#else
        uint32_t high = htonl(static_cast<uint32_t>(value >> 32));
        uint32_t low = htonl(static_cast<uint32_t>(value & 0xFFFFFFFF));
        return (static_cast<uint64_t>(low) << 32) | high;
#endif
    }

    static constexpr uint64_t fromNetwork(const uint64_t &value)
    {
#if defined(ntohll)
        return ntohll(value);
#elif defined(be64toh)
        return be64toh(value);
#else
        uint32_t high = ntohl(static_cast<uint32_t>(value >> 32));
        uint32_t low = ntohl(static_cast<uint32_t>(value & 0xFFFFFFFF));
        return (static_cast<uint64_t>(low) << 32) | high;
#endif
    }
};

template <>
struct Endian<int16_t>
{
    static constexpr int16_t toNetwork(int16_t value)
    {
        return static_cast<int16_t>(Endian<uint16_t>::toNetwork(static_cast<uint16_t>(value)));
    }

    static constexpr int16_t fromNetwork(int16_t value)
    {
        return static_cast<int16_t>(Endian<uint16_t>::fromNetwork(static_cast<uint16_t>(value)));
    }
};

template <>
struct Endian<int32_t>
{
    static constexpr int32_t toNetwork(int32_t value)
    {
        return static_cast<int32_t>(Endian<uint32_t>::toNetwork(static_cast<uint32_t>(value)));
    }

    static constexpr int32_t fromNetwork(int32_t value)
    {
        return static_cast<int32_t>(Endian<uint32_t>::fromNetwork(static_cast<uint32_t>(value)));
    }
};

template <>
struct Endian<int64_t>
{
    static constexpr int64_t toNetwork(int64_t value)
    {
        return static_cast<int64_t>(Endian<uint64_t>::toNetwork(static_cast<uint64_t>(value)));
    }

    static constexpr int64_t fromNetwork(int64_t value)
    {
        return static_cast<int64_t>(Endian<uint64_t>::fromNetwork(static_cast<uint64_t>(value)));
    }
};

template <typename TType>
constexpr TType Hton(TType value)
{
    return Endian<TType>::toNetwork(value);
}

template <typename TType>
constexpr TType Ntoh(TType value)
{
    return Endian<TType>::fromNetwork(value);
}

#endif /* ENDIAN_HPP */
