#pragma once

#include <Construction/Building.hpp>
#include <Misc/ResourcesCollection.hpp>
#include "Unit.hpp"

class Worker : public Unit
{
  public:
    enum class State
    {
        HOME,
        COMMUTING,
        RETURNING,
        WORKING
    };

  private:
    using Unit::Unit;
    std::shared_ptr<Building> m_currentTargetBuilding;
    ResourceType              m_currentResource;
    State                     m_currentState = State::HOME;

  public:
    /// Get the building the worker is currently at/going to
    /// \return The shared pointer to the building
    [[nodiscard]] std::shared_ptr<Building> getCurrentTargetBuilding() const;

    /// Change the building the defender is going to
    /// \param cTargetBuilding  The new building
    void setCurrentTargetBuilding(std::shared_ptr<Building> cTargetBuilding);

    /// Get the current state of the defender
    /// \return The state
    [[nodiscard]] State getCurrentState() const;

    /// Change the current state of the defender
    /// \param cState The new state
    void setCurrentState(State cState);

    /// Change the resource the worker is currently assigned on
    /// \return The resource
    [[nodiscard]] ResourceType getCurrentResource() const;

    /// Change the resource the worker is currently assigned on
    /// \param cResource The new resource
    void setCurrentResource(ResourceType cResource);

    float currentCollectCooldown = Globals::WORKER_COLLECTION_COOLDOWN_MS;

    /// Reset the collection cooldown to the globally defined value
    void resetCurrentCollectCooldown();
};
