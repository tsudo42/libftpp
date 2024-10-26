#ifndef OBSERVER_HPP
#define OBSERVER_HPP

#include <functional>
#include <map>

template <typename TEvent>
class Observer
{
public:
    void subscribe(const TEvent &event, const std::function<void()> &lambda)
    {
        subs_[event].push_back(lambda);
    }

    void notify(const TEvent &event)
    {
        for (auto &lambda : subs_[event])
        {
            lambda();
        }
    }

private:
    std::map<TEvent, std::vector<std::function<void()>>> subs_;
};

#endif /* OBSERVER_HPP */
