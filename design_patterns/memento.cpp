#include "design_patterns/memento.hpp"

Memento::Snapshot Memento::save()
{
    Snapshot snapshot;
    _saveToSnapshot(snapshot);
    return snapshot;
}

void Memento::load(const Memento::Snapshot &snapshot)
{
    Memento::Snapshot snapshotCopy = snapshot;
    _loadFromSnapshot(snapshotCopy);
}
