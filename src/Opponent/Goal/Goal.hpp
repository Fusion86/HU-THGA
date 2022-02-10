#pragma once

#include <Misc/Globals.hpp>

enum class UnitType;
enum class BuildingType;
class ConstructionManager;
class UnitManager;

enum class Place
{
    RANDOM,
    FRONT,
    BACK
};

template <typename type>
struct SubGoal
{
    type          goal;
    Globals::lane lane;
    Place         strategy = Place::RANDOM;

    SubGoal(type goal) : goal(goal), lane(Globals::lane::NONE) {}
    SubGoal(type goal, Globals::lane lane) : goal(goal), lane(lane) {}
    SubGoal(type goal, Globals::lane lane, Place strategy) : goal(goal), lane(lane), strategy(strategy) {}
    SubGoal(type goal, Place strategy) : goal(goal), lane(Globals::lane::NONE), strategy(strategy) {}
};

class Goal
{
    friend class UnitTrigger;
    friend class BuildingTrigger;
    std::vector<SubGoal<UnitType>>     m_spawnUnits;
    std::vector<SubGoal<BuildingType>> m_makeBuildings;
    ResourcesCollection                m_totalCost;

    float m_delay;
    float m_priorityRampupSeconds;
    int   m_priority;
    float m_remainingTimeS;
    bool  m_repeat;

  public:
    /// Create a goal
    /// \param spawnUnits Units to spawn when the goal is performed
    /// \param makeBuildings Buildings to create when the goal is performed
    /// \param priority Priority of the goal
    /// \param unitManager Unitmanager, used for calculating the cost of the goal
    /// \param constructionManager ConstructionManager, used for calculating the cost of the goal
    /// \param delay Delay until the goal is performed
    /// \param priorityRampupSeconds Time before the deadline when the priority starts ramping up
    /// \param repeat Should the goal repeat when it is met
    /// \param initialTime Initial delay(only important for repeating tasks)
    Goal(std::vector<SubGoal<UnitType>> spawnUnits, std::vector<SubGoal<BuildingType>> makeBuildings, int priority,
         UnitManager& unitManager, ConstructionManager& constructionManager, float delay = 0.0,
         float priorityRampupSeconds = 0.0, bool repeat = false, float initialTime = -1.0);

    /// Get the cost of the goal
    /// \return The calculated cost of the Goal
    virtual ResourcesCollection getCost();

    /// Get the current cost of the goal
    ///
    /// \details Note that if this method is called twice within one update, a deltatime should only be passed once
    /// \param deltaTime Deltatime, used to calculate remaining times until deadline
    /// \return The current calculated priority
    virtual int getPriority(float deltaTime);

    /// Get the units to be spawned when the goal is met
    /// \return The vector of unit goals
    [[nodiscard]] const std::vector<SubGoal<UnitType>>&     getSpawnUnits() const;

    /// Get the buildings to be created when the goal is met
    /// \return The vector of building goals
    [[nodiscard]] const std::vector<SubGoal<BuildingType>>& getMakeBuildings() const;

    /// Called when the goal was performed, used to reset remaining times
    virtual void goalPerformed();

    /// Check if the goal is repeating
    /// \return True if the goal is repeating
    bool         isRepeating();
};
