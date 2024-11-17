#ifndef IVECTOR2_HPP
#define IVECTOR2_HPP

#include <type_traits>
#include <cmath>
#include <ostream>

template <typename TType>
struct IVector2
{
    TType x;
    TType y;

    IVector2() = default;
    explicit IVector2(TType x_, TType y_) : x(x_), y(y_) {}
    virtual ~IVector2() = default;

    IVector2<TType> operator+(const IVector2<TType> &other) const
    {
        return IVector2<TType>(x + other.x, y + other.y);
    }

    IVector2<TType> operator-(const IVector2<TType> &other) const
    {
        return IVector2<TType>(x - other.x, y - other.y);
    }

    IVector2<TType> operator*(const IVector2<TType> &other) const
    {
        return IVector2<TType>(x * other.x, y * other.y);
    }

    IVector2<TType> operator/(const IVector2<TType> &other) const
    {
        return IVector2<TType>(x / other.x, y / other.y);
    }

    bool operator==(const IVector2<TType> &other) const
    {
        return x == other.x && y == other.y;
    }

    bool operator!=(const IVector2<TType> &other) const
    {
        return !(*this == other);
    }

    auto length() const
    {
        static_assert(std::is_arithmetic<TType>::value, "TType must be an arithmetic type.");

        if constexpr (std::is_integral<TType>::value)
        {
            return std::sqrt(static_cast<float>(x) * x + static_cast<float>(y) * y);
        }
        else
        {
            return std::sqrt(x * x + y * y);
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
                return IVector2<float>(0.0f, 0.0f);
            }
            else
            {
                return IVector2<TType>(static_cast<TType>(0), static_cast<TType>(0));
            }
        }

        if constexpr (std::is_integral<TType>::value)
        {
            return IVector2<float>(static_cast<float>(x) / len, static_cast<float>(y) / len);
        }
        else
        {
            return IVector2<TType>(x / len, y / len);
        }
    }

    TType dot(const IVector2<TType> &other) const
    {
        static_assert(std::is_arithmetic<TType>::value, "TType must be an arithmetic type.");
        return x * other.x + y * other.y;
    }

    IVector2<TType> cross() const
    {
        static_assert(std::is_arithmetic<TType>::value, "TType must be an arithmetic type.");
        return IVector2<TType>(-y, x);
    }
};

#endif /* IVECTOR2_HPP */
