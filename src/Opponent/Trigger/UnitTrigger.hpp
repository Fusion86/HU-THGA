#pragma once

#include <list>
#include <Misc/Globals.hpp>

#include <Opponent/Goal/Goal.hpp>

class UnitTrigger
{
  public:
    /// Enum of UnitTrigger Types
    enum class Type
    {
        SPAWNED,
        KILLED,
        BOTH
    };

  private:
    std::list<UnitType> m_triggeredUnits;
    Globals::side       m_owner;
    Type                m_triggerType;
    Goal                m_goal;
    bool                m_performOnSameLane;

  public:
    /// Create a unit trigger
    /// \param triggeredBuildings Units this trigger is performed for
    /// \param triggerType Type of trigger
    /// \param owner Owner for which units the trigger should be activated
    /// \param goal Goal to be reached when the trigger is activated
    /// \param mPerformOnSameLane If true, the goal is adjusted to match the lane the trigger happened on
    UnitTrigger(std::list<UnitType> triggeredUnits, UnitTrigger::Type triggerType, Globals::side owner, Goal goal,
                bool mPerformOnSameLane);

    /// Check if the trigger matches the given values
    ///
    /// \details Note that this method also changes the lanes in the stored goal, since it's assumed that the goal is retrieved immediately after
    /// \param unit Unit that was killed/spawned
    /// \param triggerType Event that happened to the unit
    /// \param lane Lane the  unit was/is on
    /// \param owner Owner of the unit
    /// \return Ttrue if the trigger matches the above conditions
    bool checkTrigger(UnitType unit, UnitTrigger::Type triggerType, Globals::lane lane, Globals::side owner);

    /// Get a shared pointer to the stored goal
    ///
    /// \details This shared pointer is created from the data in the stored goal, not pointing to the stored goal
    /// \return Shared pointer to a new goal with the information of the stored goal
    std::shared_ptr<Goal> getGoal();
};
