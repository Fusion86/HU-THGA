#pragma once

#include <memory>

class Unit;
struct UnitListener
{
    /// Listener function that is called when any unit is killed
    /// \param unit The unit that was killed
    virtual void onUnitSpawned(std::shared_ptr<Unit> unit) = 0;
    /// Listener function that is called when any unit is spawned
    /// \param unit The unit that was spawned
    virtual void onUnitKilled(std::shared_ptr<Unit> unit)  = 0;
};