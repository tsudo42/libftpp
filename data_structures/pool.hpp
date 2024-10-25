#ifndef POOL_HPP
#define POOL_HPP

#include <unordered_set>
#include <stack>
#include <stdexcept>

template <typename TType>
class Pool
{
public:
    class Object
    {
    public:
        Object(Pool &pool, TType *ptr)
            : pool_(&pool), ptr_(ptr) {}

        Object(const Object &) = delete;
        Object &operator=(const Object &) = delete;

        Object(Object &&other)
        {
            pool_ = other.pool_;
            ptr_ = other.ptr_;
            other.ptr_ = nullptr;
            other.pool_ = nullptr;
        }
        Object &operator=(Object &&other)
        {
            if (this != &other)
            {
                release();
                pool_ = other.pool_;
                ptr_ = other.ptr_;
                other.ptr_ = nullptr;
                other.pool_ = nullptr;
            }
            return *this;
        }

        ~Object() { release(); };

        TType *operator->() { return ptr_; }
        const TType *operator->() const { return ptr_; }
        TType &operator*() { return *ptr_; }
        const TType &operator*() const { return *ptr_; }
        TType *get() { return ptr_; }
        const TType *get() const { return ptr_; }

    private:
        Pool *pool_;
        TType *ptr_;

        void release()
        {
            if (ptr_ && pool_)
            {
                pool_->release(*this);
                ptr_ = nullptr;
                pool_ = nullptr;
            }
        }
    };

    class NotEnoughObjects : public std::runtime_error
    {
    public:
        NotEnoughObjects() : std::runtime_error("No available objects in the pool.") {}
        virtual ~NotEnoughObjects() = default;
    };

    class TooManyActiveObjects : public std::runtime_error
    {
    public:
        TooManyActiveObjects() : std::runtime_error("Not enough available objects to reduce.") {}
        virtual ~TooManyActiveObjects() = default;
    };

    Pool() = default;
    explicit Pool(const std::size_t &numberOfObjectStored)
    {
        resize(numberOfObjectStored);
    }

    ~Pool()
    {
        for (TType *obj : objs_in_use_)
        {
            obj->~TType();
        }
        for (TType *obj : objs_all_)
        {
            ::operator delete(static_cast<void *>(obj));
        }
    }

    void resize(const std::size_t numberOfObjectStored)
    {
        if (numberOfObjectStored >= objs_all_.size())
        {
            for (std::size_t i = objs_all_.size(); i < numberOfObjectStored; ++i)
            {
                TType *obj = static_cast<TType *>(::operator new(sizeof(TType)));
                objs_all_.insert(obj);
                objs_available_.push(obj);
            }
        }
        else
        {
            std::size_t numToRemove = objs_all_.size() - numberOfObjectStored;
            while (numToRemove > 0 && !objs_available_.empty())
            {
                TType *obj = objs_available_.top();
                objs_available_.pop();
                ::operator delete(static_cast<void *>(obj));
                objs_all_.erase(obj);
                --numToRemove;
            }
            if (numToRemove > 0)
            {
                throw TooManyActiveObjects();
            }
        }
    }

    template <typename... TArgs>
    Object acquire(TArgs &&...args)
    {
        if (objs_available_.empty())
        {
            throw NotEnoughObjects();
        }

        TType *obj = objs_available_.top();
        objs_available_.pop();
        try
        {
            new (obj) TType(std::forward<TArgs>(args)...);
            objs_in_use_.insert(obj);
        }
        catch (...)
        {
            objs_available_.push(obj);
            throw;
        }
        return Object(*this, obj);
    }

private:
    std::unordered_set<TType *> objs_all_;
    std::unordered_set<TType *> objs_in_use_;
    std::stack<TType *> objs_available_;

    void release(Object &obj)
    {
        TType *ptr = obj.get();
        if (ptr)
        {
            ptr->~TType();
            objs_in_use_.erase(ptr);
            objs_available_.push(ptr);
        }
    }
};

#endif /* POOL_HPP */
