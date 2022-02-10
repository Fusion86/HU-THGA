#pragma once

#include <list>
#include <Misc/Globals.hpp>
#include <Opponent/Goal/Goal.hpp>

enum class BuildingType;



class BuildingTrigger
{
  public:
    /// Enum of BuildingTrigger Types
    enum class Type
    {
        CREATED,
        DESTROYED,
        BOTH
    };

  private:
    std::list<BuildingType> m_triggeredBuildings;
    Type                    m_triggerType;
    Globals::side           m_owner;
    Goal                    m_goal;
    bool                    m_performOnSameLane;

  public:
    /// Create a building trigger
    /// \param triggeredBuildings Buildings this trigger is performed on
    /// \param triggerType Type of trigger
    /// \param owner Owner for which buildings the trigger should be activated
    /// \param goal Goal to be reached when the trigger is activated
    /// \param mPerformOnSameLane If true, the goal is adjusted to match the lane the trigger happened on
    BuildingTrigger(std::list<BuildingType> triggeredBuildings, BuildingTrigger::Type triggerType, Globals::side owner,
                    Goal goal, bool mPerformOnSameLane = true);

    /// Check if the trigger matches the given values
    ///
    /// \details Note that this method also changes the lanes in the stored goal, since it's assumed that the goal is retrieved immediately after
    /// \param building Building that was destroyed/created
    /// \param triggerType Event that happened to the building
    /// \param lane Lane the  building was/is on
    /// \param owner Owner of the building
    /// \return Ttrue if the trigger matches the above conditions
    bool checkTrigger(BuildingType building, BuildingTrigger::Type triggerType, Globals::lane lane, Globals::side owner);

    /// Get a shared pointer to the stored goal
    ///
    /// \details This shared pointer is created from the data in the stored goal, not pointing to the stored goal
    /// \return Shared pointer to a new goal with the information of the stored goal
    std::shared_ptr<Goal> getGoal();
};
