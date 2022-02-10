#pragma once

#include <memory>
class Building;

struct BuildingListener
{
    /// Listener function that is called when any building is destroyed
    /// \param building The building that was destroyed
    virtual void onBuildingDestroyed(std::shared_ptr<Building> building) = 0;

    /// Listener function that is called when any building is created
    /// \param building The building that was created
    virtual void onBuildingCreated(std::shared_ptr<Building> building)   = 0;
};