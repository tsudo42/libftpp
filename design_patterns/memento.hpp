#ifndef MEMENTO_HPP
#define MEMENTO_HPP

#include "data_structures/data_buffer.hpp"

class Memento
{
public:
    class Snapshot : public DataBuffer
    {
    public:
        Snapshot() = default;
        virtual ~Snapshot() = default;
    };

    Memento() = default;
    virtual ~Memento() = default;

    Snapshot save();
    void load(const Snapshot &snapshot);

protected:
    virtual void _saveToSnapshot(Snapshot &snapshot) = 0;
    virtual void _loadFromSnapshot(Snapshot &snapshot) = 0;
};

#endif /* MEMENTO_HPP */
