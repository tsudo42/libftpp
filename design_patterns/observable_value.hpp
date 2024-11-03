#ifndef OBSERVABLE_VALUE_HPP
#define OBSERVABLE_VALUE_HPP

#include <functional>

template <typename TType>
class ObservableValue
{
public:
    void set(TType value)
    {
        value_ = value;
        notify();
    }

    const TType &get() const
    {
        return value_;
    }

    void subscribe(const std::function<void(const TType &)> &lambda)
    {
        subs_.push_back(lambda);
    }

    void notify() const
    {
        for (auto &lambda : subs_)
        {
            lambda(value_);
        }
    }

private:
    TType value_{};
    std::vector<std::function<void(const TType &)>> subs_;
};

#endif /* OBSERVABLE_VALUE_HPP */
