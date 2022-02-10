#pragma once

#include <Unit/Unit.hpp>
#include <Construction/Building.hpp>

class Defender : public Unit
{
  public:
    enum class State
    {
        IDLE,
        COMMUTING,
        DEFENDING
    };

  private:
    using Unit::Unit;
    std::shared_ptr<Building> m_currentTargetBuilding;
    State                     m_currentState = State::IDLE;

  public:
    /// Get the building the defender is currently at/going to
    /// \return The shared pointer to the building
    [[nodiscard]] std::shared_ptr<Building>& getCurrentTargetBuilding();

    /// Change the building the defender is going to
    /// \param currentTargetBuilding  The new building
    void                setCurrentTargetBuilding(const std::shared_ptr<Building>& currentTargetBuilding);

    /// Get the current state of the defender
    /// \return The state
    [[nodiscard]] State getCurrentState() const;

    /// Change the current state of the defender
    /// \param newState The new state
    void                setCurrentState(State newState);
};
