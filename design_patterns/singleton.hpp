#ifndef SINGLETON_HPP
#define SINGLETON_HPP

#include <stdexcept>

template <typename TType>
class Singleton
{
public:
    class NotInstanciated : public std::runtime_error
    {
    public:
        virtual ~NotInstanciated() = default;
        NotInstanciated() : std::runtime_error("Instance not yet created") {}
    };

    class AlreadyInstanciated : public std::runtime_error
    {
    public:
        virtual ~AlreadyInstanciated() = default;
        AlreadyInstanciated() : std::runtime_error("Instance already created") {}
    };

    static TType *instance(void)
    {
        if (instance_ == nullptr)
        {
            throw NotInstanciated();
        }
        return instance_;
    }

    template <typename... TArgs>
    static void instanciate(TArgs... args)
    {
        if (instance_ != nullptr)
        {
            throw AlreadyInstanciated();
        }
        instance_ = new TType(std::forward<TArgs>(args)...);
    }

private:
    static TType *instance_;
};

template <typename TType>
TType *Singleton<TType>::instance_ = nullptr;

#endif /* SINGLETON_HPP */
