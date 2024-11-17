#ifndef COMPRESSED_VECTOR_HPP
#define COMPRESSED_VECTOR_HPP

#include <initializer_list>
#include <vector>
#include <algorithm>

template <typename TType>
class CoordinateCompressor
{
public:
    CoordinateCompressor() = default;

    template <typename TInputIt>
    CoordinateCompressor(TInputIt first, TInputIt last)
    {
        insert(first, last);
    }

    CoordinateCompressor(std::initializer_list<TType> ilist)
    {
        insert(ilist);
    }

    void insert(const TType &value)
    {
        values_.push_back(value);
    }

    template <typename TInputIt>
    void insert(TInputIt first, TInputIt last)
    {
        values_.insert(values_.end(), first, last);
    }

    void insert(std::initializer_list<TType> ilist)
    {
        values_.insert(values_.end(), ilist.begin(), ilist.end());
    }

    std::vector<std::size_t> compress() const
    {
        std::vector<TType> unique_vals = values_;
        std::sort(unique_vals.begin(), unique_vals.end());
        unique_vals.erase(std::unique(unique_vals.begin(), unique_vals.end()), unique_vals.end());

        std::vector<std::size_t> result;
        result.reserve(values_.size());

        for (const auto &value : values_)
        {
            auto it = std::lower_bound(unique_vals.begin(), unique_vals.end(), value);
            result.push_back(std::distance(unique_vals.begin(), it));
        }
        return result;
    }

private:
    std::vector<TType> values_;
};

#endif /* COMPRESSED_VECTOR_HPP */
