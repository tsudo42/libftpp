#ifndef IVECTOR3_HPP
#define IVECTOR3_HPP

#include <type_traits>
#include <ostream>

template <typename TType>
struct IVector3
{
    TType x;
    TType y;
    TType z;

    IVector3() = default;
    explicit IVector3(TType x_, TType y_, TType z_) : x(x_), y(y_), z(z_) {}
    virtual ~IVector3() = default;

    IVector3<TType> operator+(const IVector3<TType> &other) const
    {
        return IVector3<TType>(x + other.x, y + other.y, z + other.z);
    }

    IVector3<TType> operator-(const IVector3<TType> &other) const
    {
        return IVector3<TType>(x - other.x, y - other.y, z - other.z);
    }

    IVector3<TType> operator*(const IVector3<TType> &other) const
    {
        return IVector3<TType>(x * other.x, y * other.y, z * other.z);
    }

    IVector3<TType> operator/(const IVector3<TType> &other) const
    {
        return IVector3<TType>(x / other.x, y / other.y, z / other.z);
    }

    bool operator==(const IVector3<TType> &other) const
    {
        return x == other.x && y == other.y && z == other.z;
    }

    bool operator!=(const IVector3<TType> &other) const
    {
        return !(*this == other);
    }

    auto length() const
    {
        static_assert(std::is_arithmetic<TType>::value, "TType must be an arithmetic type.");

        if constexpr (std::is_integral<TType>::value)
        {
            return std::sqrt(static_cast<float>(x) * x + static_cast<float>(y) * y + static_cast<float>(z) * z);
        }
        else
        {
            return std::sqrt(x * x + y * y + z * z);
        }
    }

    auto normalize() const
    {
        static_assert(std::is_arithmetic<TType>::value, "TType must be an arithmetic type.");

        auto len = length();
        if (len == 0)
        {
            if constexpr (std::is_integral<TType>::value)
            {
                return IVector3<float>(0.0f, 0.0f, 0.0f);
            }
            else
            {
                return IVector3<TType>(static_cast<TType>(0), static_cast<TType>(0), static_cast<TType>(0));
            }
        }

        if constexpr (std::is_integral<TType>::value)
        {
            return IVector3<float>(static_cast<float>(x) / len, static_cast<float>(y) / len, static_cast<float>(z) / len);
        }
        else
        {
            return IVector3<TType>(x / len, y / len, z / len);
        }
    }

    TType dot(const IVector3<TType> &other) const
    {
        static_assert(std::is_arithmetic<TType>::value, "TType must be an arithmetic type.");
        return x * other.x + y * other.y + z * other.z;
    }

    IVector3<TType> cross(const IVector3<TType> &other) const
    {
        static_assert(std::is_arithmetic<TType>::value, "TType must be an arithmetic type.");
        return IVector3<TType>(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x);
    }
};

#endif /* IVECTOR3_HPP */
